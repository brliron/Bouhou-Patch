#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<Windows.h>
#include	<Shlwapi.h>
#include	<stdio.h>
#include	<wchar.h>
#include	<string.h>
#include	<errno.h>

#include	"chars.hpp"
#include	"Reader.hpp"
#include	"Output.hpp"
#include	"Bitmap.hpp"
#include	"BackgroundTasksManager.hpp"
#include	"ATexturesManager.hpp"
#include	"Boho/TexturesFlags.hpp"
#ifndef DISABLE_END_MANAGER
# include	"EndManager.hpp"
#endif /* !DISABLE_END_MANAGER */

std::vector<Char>	chars;

static std::list<Char>*	chars_list = NULL;




/*
** Find a Char in the characters table from its unicode value.
*/
Char*	Char::get(wchar_t c)
{
  for (unsigned int i = 0; i < chars.size(); i++)
    if (chars[i].c == c)
      return &chars[i];
  return NULL;
}

/*
** Some functions to compare 2 characters using their position.
*/
bool	float_cmp(float f1, float f2)
{
  int		n1;
  int		n2;

  n1 = f1 * 10000;
  n2 = f2 * 10000;
  return n1 == n2 || n1 + 1 == n2 || n1 == n2 + 1;
}

bool	Char::cmp_by_pos(float tx1, float ty1) const
{
  return float_cmp(tx1, this->tx1) && float_cmp(ty1, this->ty1);
}

bool	Char::cmp_by_pos(const Char& c) const
{
  return this->cmp_by_pos(c.tx1, c.ty1);
}


/*
** Find a Char in the characters table from its position in the texture file.
*/
Char*	Char::get(float u, float v)
{
  for (unsigned int i = 0; i < chars.size(); i++)
    if (chars[i].cmp_by_pos(u, v))
      return &chars[i];
  return NULL;
}


/*
** Compare 2 characters using their unicode value.
** Used for sorting.
*/
bool	Char::operator<(const Char& c) const
{
  if (this->c == L'\0')
    return false;
  if (c.c == L'\0')
    return true;
  return this->c < c.c;
}


bool	Char::operator==(const Char& c) const
{
  return this->c   == c.c   &&
	 this->tx1 == c.tx1 &&
	 this->ty1 == c.ty1 &&
	 this->tx2 == c.tx2 &&
	 this->ty2 == c.ty2 &&
	 this->sy  == c.sy  &&
	 this->sw  == c.sw  &&
	 this->sh  == c.sh;
}

bool	Char::operator!=(const Char& c) const
{
  return !(*this == c);
}


/*
** Copy the filled fields of src in this.
*/
void	Char::update(const Char& src)
{
  if (src.c != L'\0')
    this->c = src.c;
  if (src.tx1 != -1)
    this->tx1 = src.tx1;
  if (src.tx2 != -1)
    this->tx2 = src.tx2;
  if (src.ty1 != -1)
    this->ty1 = src.ty1;
  if (src.ty2 != -1)
    this->ty2 = src.ty2;
  if (src.sy != -1)
    this->sy = src.sy;
  if (src.sw != 0)
    this->sw = src.sw;
  if (src.sh != 0)
    this->sh = src.sh;
}



/*
** If a character is unknown, try to find it in the characters directory.
*/
bool			Char::update_from_bmp_filename(int flags)
{
  WCHAR			filename[MAX_PATH];
  HANDLE		find_handle;
  WIN32_FIND_DATAW	findFileData;

  wsprintfW(filename, L"characters\\?_%d_%d.bmp", (int)(this->tx1 * 10000), (int)(this->ty1 * 10000));
  find_handle = FindFirstFileW(filename, &findFileData);
  if (find_handle != INVALID_HANDLE_VALUE)
    {
      do
	{
	  if (findFileData.cFileName[0] != '-')
	    {
	      this->c = findFileData.cFileName[0];
	      this->log(flags);
	      return true;
	    }
	} while (FindNextFileW(find_handle, &findFileData));
      FindClose(find_handle);
    }
  return false;
}

/*
** If a character have been found or updated, we update its representation in chars_list and return true.
*/
bool	Char::store(int flags)
{
  if (this->c == L'\0')
    {
      if (this->update_from_bmp_filename(flags))
	return true;
      for (Char& it : *chars_list)
	if (*this == it)
	  return false;
	else if (this->cmp_by_pos(it))
	  {
	    it.update(*this);
	    return true;
	  }
      chars_list->push_back(*this);
      if ((flags & CHARBUFF__DISABLE_BMP_SAVE) == 0)
	this->store_as_bmp();
      return true;
    }

  for (Char& it : *chars_list)
    {
      if (it == *this)
	return false;
      else if (this->cmp_by_pos(it))
	{
	  it.update(*this);
	  chars_list->sort();
	  return true;
	}
    }
  for (std::list<Char>::iterator it = chars_list->begin(); it != chars_list->end(); ++it)
    if (it->c > this->c || it->c == L'\0')
      {
	chars_list->insert(it, *this);
	return true;
      }
  chars_list->push_back(*this);
  return true;
}

/*
** Update chars_list and log it to chars_dump.csv if needed
*/
void	Char::log(int flags)
{
  if (chars_list == NULL)
    {
      chars_list = new std::list<Char>;
      for (unsigned int i = 0; i < chars.size(); i++)
        chars[i].store(flags);
    }
  if (this->store(flags) && (flags & CHARBUFF__DISABLE_CHARS_LIST) == 0)
    Char::print_logs_to_file();
}

/*
** Saves chars_list to the file chars_dump.csv.
*/
void	Char::print_logs_to_file()
{
  FILE	*fd;

  fd = _wfopen(L"chars_dump.csv", L"w, ccs=UNICODE");
  if (fd == NULL)
    {
      Reader::get().dispMessageF(L"ERR_OPEN", L"chars_dump.csv", _wcserror(errno));
      return ;
    }
  for (Char& it : *chars_list)
    fwprintf(fd, L"%c,%f,%f,%f,%f,%d,%d,%d\n", it.c ? it.c : L'?', it.tx1, it.tx2, it.ty1, it.ty2, it.sy, it.sw, it.sh);
  fclose(fd);
#ifndef DISABLE_END_MANAGER
  endManager->addFile(L"chars_dump.csv");
#endif /* !DISABLE_END_MANAGER */
}


const DisplayedChar&	DisplayedChar::operator=(const Char& src)
{
  this->c   = src.c;
  this->tx1 = src.tx1;
  this->tx2 = src.tx2;
  this->ty1 = src.ty1;
  this->ty2 = src.ty2;
  this->sy  = src.sy;
  this->sw  = src.sw;
  this->sh  = src.sh;
  return *this;
}


/*
** This define disables the support of BMP saving.
** It is useless in the final release, but makes linkage easier for my various tools using this file.
*/
#ifndef DISABLE_BMP
// Callback function for Char::store_as_bmp
static void	write_bmp_to_file(const Bitmap* font, const Char c, LPWSTR filename)
{
  Bitmap	bmp;

  bmp = *font;
  bmp.shrink(c.tx1 * 2048, c.ty1 * 2048, (c.tx2 - c.tx1) * 2048, (c.ty2 - c.ty1) * 2048);
  if (bmp.save_24bpp(filename) == false)
    Output::printf(L"Warning : couldn't save bitmap as %s.\n"
		   "the character %d_%d is unknown and could not be saved.", filename);
# ifndef DISABLE_END_MANAGER
  else
    endManager->addFile(filename);
# endif /* !DISABLE_END_MANAGER */
  free(filename);
}

// Saves an unknown character as a BMP file.
void			Char::store_as_bmp() const
{
  static Bitmap*	font = NULL;
  WCHAR			out_filename[MAX_PATH];

  wsprintfW(out_filename, L"characters\\%c_%d_%d.bmp", this->c ? this->c : L'-', (int)(this->tx1 * 10000), (int)(this->ty1 * 10000));
# ifndef DISABLE_END_MANAGER
  if (endManager->isFileInArchive(out_filename) || PathFileExistsW(out_filename))
    return ;
# endif /* !DISABLE_END_MANAGER */
  if (!PathIsDirectoryW(L"characters"))
    CreateDirectoryW(L"characters", NULL);
  if (font == NULL)
    {
      LPD3DXBUFFER	raw_bmp;

      font = new Bitmap;
# ifndef LOAD_FONT_BITMAP_FROM_FILE
      if (D3DXSaveTextureToFileInMemory(&raw_bmp, D3DXIFF_BMP, (LPDIRECT3DTEXTURE9)get_texture_by_flag(Boho::TexturesFlags::FONT), NULL) != D3D_OK ||
	  font->load_from_memory(static_cast<unsigned char*>(raw_bmp->GetBufferPointer())) == false)
	{
	  Output::printf(L"Warning : couldn't load the bitmap font.\n"
			 "the character %d_%d is unknown and could not be saved.", (int)(this->tx1 * 10000), (int)(this->ty1 * 10000));
	  delete font;
	  font = NULL;
	  return ;
	}
# else
      font->load("font.bmp");
# endif
    }
  BackgroundTasksManager::get().add(write_bmp_to_file, font, *this, wcsdup(out_filename));
}
#else
void	Char::store_as_bmp() const
{}
#endif /* !DISABLE_BMP */
