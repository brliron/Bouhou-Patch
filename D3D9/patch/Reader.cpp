#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<Windows.h>
#include	<Shlwapi.h>
#include	<stdio.h>
#include	<string.h>
#include	"Reader.hpp"
#include	"ParseText.hpp"
#include	"chars.hpp"
#include	"APatch.hpp"
#include	"DropdownBox.h"

Reader*	Reader::instance = NULL;
extern std::vector<Char>	chars;

Reader::Reader()
  : _initCalled(false),
    messages({
	{L"ERR_MESSAGES_UNKNOWN_KEY",	    {L"", L"messages.txt line %d : unknown key %s.", L"%d", L"%s"}},
	{L"ERR_WRONG_PERCENTS",		    {L"", L"%s line %d : the percent flags are wrong.", L"%s", L"%d"}},
	{L"ERR_LINE_TOO_LONG",		    {L"", L"Warning : %s : the %dth line is too long (more than 512 characters).", L"%s", L"%d"}},
	{L"ERR_SYNTAX",			    {L"", L"Warning : %s : syntax error line %d.", L"%s", L"%d"}},
	{L"ERR_TRANSLATIONS_DIR_NOT_FOUND", {L"", L"The directory \"translation\" doesn't exist.", L"%%"}},
	{L"ERR_DIR_NOT_FOUND",		    {L"", L"The directory \"%s\" doesn't exist.\n"
						   "It probably means you don't have the translations for this game in the specified language.", L"%s"}},
	{L"ERR_FILE_NOT_FOUND",		    {L"", L"The file \"%s\" doesn't exist.", L"%s"}},
	{L"ERR_OPEN",			    {L"", L"Could not open %s : %s", L"%s", L"%s"}},
	{L"ERR_PARSETEXT_WRONG_MODE",	    {L"", L"Warning : wrong value for ParseText:mode in translations\\config.ini\n"
						   "Defaulting to TRANSLATE_FIRST.", L"%%"}},
    })
{}

void	Reader::readMessages()
{
  FILE*	fd;

  fd = _wfopen(this->getFilePath(L"messages.txt"), L"r, ccs=UNICODE");
  if (fd == NULL)
    return ;

  WCHAR	buff[4096 + 31 + 6]; // 31 is the size of the biggest key, plus the equal sign. 6 is the size of "_TITLE".
  int	n = 0;

  while (fgetws(buff, 4096 + 31 + 6, fd))
    {
      n++;
      WCHAR*	value = wcschr(buff, L'=');
      bool	isTitle = false;

      if (value == NULL)
	{
	  this->dispMessageF(L"ERR_SYNTAX", L"messages.txt", n);
	  continue ;
	}
      *value = L'\0';
      value++;
      if (wcscmp(buff + wcslen(buff) - 6, L"_TITLE") == 0)
	{
	  buff[wcslen(buff) - 6] = L'\0';
	  isTitle = true;
	}
      if (this->messages.count(buff) == 0)
	{
	  this->dispMessageF(L"ERR_MESSAGES_UNKNOWN_KEY", n, buff);
	  continue ;
	}
      std::vector<std::wstring>&	v = this->messages[buff];
      if (isTitle == false && this->checkPercents(value, std::vector<std::wstring>(v.begin() + 2, v.end())) == false)
	{
	  this->dispMessageF(L"ERR_WRONG_PERCENTS", L"messages.txt", n);
	  continue ;
	}

      if (value[wcslen(value) - 1] == L'\n')
	value[wcslen(value) - 1] = L'\0';
      for (WCHAR* c = value; *c; c++)
	if (*c == L'\\' && (c[1] == L'n' || c[1] == L'\\'))
	  {
	    if (c[1] == L'n')
	      *c = L'\n';
	    else
	      *c = L'\\';
	    wmemmove(c + 1, c + 2, wcslen(c) - 1);
	  }
      if (isTitle)
	v[0] = value;
      else
	v[1] = value;
    }
  fclose(fd);
}

bool		Reader::checkPercents(LPCWSTR value, const std::vector<std::wstring>& v) const
{
  const WCHAR*	ptr;

  if (v[0] == L"%%")
    return true;

  ptr = value;
  for (const std::wstring& flag : v)
    {
      do
	ptr = wcschr(ptr, L'%');
      while (ptr && (ptr += 2)[-1] == L'%');
      if (!ptr || ptr[-1] != flag[1])
	return false;
    }
  do
    ptr = wcschr(ptr, L'%');
  while (ptr && (ptr += 2)[-1] == L'%');
  if (ptr)
    return false;
  return true;
}


const std::wstring&	Reader::getMessage(const std::wstring& key) const
{
  return this->messages.at(key)[1];
}

void			Reader::dispMessage(const std::wstring& key) const
{
  const std::vector<std::wstring>&	v = this->messages.at(key);

  APatch::get().MessageBox(v[1].c_str(), !v[0].empty() ? v[0].c_str() : NULL);
}


static void	unescape_string(LPWSTR str)
{
  for (int i = 0; str[i]; i++)
    if (str[i] == L'\\')
      {
	if (str[i + 1] == L'\\' || str[i + 1] == L'=' || str[i + 1] == L'n')
	  memmove(str + i, str + i + 1, wcslen(str + i) * 2);
	if (str[i] == L'n')
	  str[i] = L'\n';
      }
}

static void	parse_line(LPWSTR line, int)
{
  WCHAR*	s1;
  WCHAR*	s2;

  if ((line[0] == L'#' && line[1] == L'#') || line[0] == L'\0')
    return ;
  s1 = line;
  while ((s2 = wcschr(line, L'=')) != NULL && (s1 == s2 || s2[-1] == L'\\'));
  if (s2)
    {
      *s2 = L'\0';
      s2++;
    }
  if (s2 && wcscmp(s2, L"<NO TRANSLATION>") == 0)
    s2 = NULL;
  unescape_string(s1);
  if (s2)
    unescape_string(s2);
  ParseText::strings.push_back(wcsdup(s1));
  if (s2)
    ParseText::strings.push_back(wcsdup(s2));
  else
    ParseText::strings.push_back(NULL);
}

static void	read_file(LPCWSTR file, void (*callback)(LPWSTR, int))
{
  FILE*		fd;
  WCHAR		buff[512];
  bool		missing_nl = false;
  int		line_num;

  fd = _wfopen(file, L"r, ccs=UNICODE");
  if (fd == NULL)
    return ;
  line_num = 0;
  while (fgetws(buff, 512, fd))
    {
      if (missing_nl)
	{
	  Reader::get().dispMessageF(L"ERR_LINE_TOO_LONG", file, line_num);
	  return ;
	}
      if (buff[wcslen(buff) - 1] == L'\n')
	buff[wcslen(buff) - 1] = L'\0';
      else
	missing_nl = true;
      callback(buff, line_num);
      line_num++;
    }
  fclose(fd);
}

void			Reader::readFiles()
{
  HANDLE		hFind;
  WCHAR			searchPath[PATH_MAX];
  WCHAR			relPath[PATH_MAX];
  WIN32_FIND_DATAW	file;

  // At the beginning, I thought the filenames should either be a single script.txt or a collection of script_something.txt.
  // I think it may be convenient to put these files in a dedicated directory, with any filename.

  // New way: script\*.txt
  wcscpy(searchPath, this->getFilePath(L"script\\*.txt"));
  wcscpy(relPath, L"script\\");
  hFind = FindFirstFileW(searchPath, &file);
  if (hFind != INVALID_HANDLE_VALUE)
    {
      do
	{
	  if ((file.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_ENCRYPTED | FILE_ATTRIBUTE_OFFLINE)) ||
	      wcscmp(file.cFileName, L"end.txt") == 0)
	    continue ;
	  wcscpy(relPath + wcslen(L"script\\"), file.cFileName);
	  read_file(this->getFilePath(relPath), parse_line);
	} while(FindNextFileW(hFind, &file));
      FindClose(hFind);
    }

  // Legacy way: script*.txt
  wcscpy(searchPath, this->getFilePath(L"script*.txt"));
  hFind = FindFirstFileW(searchPath, &file);
  if (hFind != INVALID_HANDLE_VALUE)
    {
      do
	{
	  if ((file.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_ENCRYPTED | FILE_ATTRIBUTE_OFFLINE)) ||
	      wcscmp(file.cFileName, L"script_end.txt") == 0)
	    continue ;
	  read_file(this->getFilePath(file.cFileName), parse_line);
	} while(FindNextFileW(hFind, &file));
      FindClose(hFind);
    }

  // Finally, end files.
  read_file(this->getFilePath(L"script_end.txt"), parse_line);
  read_file(this->getFilePath(L"script\\end.txt"), parse_line);

  ParseText::strings.push_back(NULL);
}

// Reads a line from chars.csv.
static void	chars_list_read_line(LPWSTR line, int line_num)
{
  Char		c;

  if (line_num == 0)
    return ;
  if (swscanf(line, L"%c,%f,%f,%f,%f,%d,%d,%d", &c.c, &c.tx1, &c.tx2, &c.ty1, &c.ty2, &c.sy, &c.sw, &c.sh) != 8)
    {
      Reader::get().dispMessageF(L"ERR_SYNTAX", L"chars.csv", line_num);
      return ;
    }
  chars.push_back(c);
}

void	Reader::readCharsList()
{
  read_file(this->getFilePath(L"chars.csv"), chars_list_read_line);
}



bool	Reader::getPaths(LPCWSTR game)
{
  WCHAR	curDir[MAX_PATH];
  WCHAR	lang[21];

  GetModuleFileNameW(GetModuleHandle(NULL), curDir, MAX_PATH);
  curDir[MAX_PATH - 1] = L'\0';
  PathRemoveFileSpecW(curDir);
  PathRemoveBackslashW(curDir);

  GetPrivateProfileStringW(L"global", L"lang", L"", lang, 21, L"translations\\games.ini");
  if (lang[0] == L'\0')
    {
      HANDLE			hFind;
      WIN32_FIND_DATAW		find;
      std::vector<LPWSTR>	languages;
      int			res;

      hFind = FindFirstFileW(L"translations\\*", &find);
      do
	{
	  if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
	      wcsncmp(find.cFileName, game, wcslen(game)) == 0 &&
	      find.cFileName[wcslen(game)] == L'_')
	    languages.push_back(wcsdup(find.cFileName + wcslen(game) + 1));
	} while (FindNextFileW(hFind, &find));
      FindClose(hFind);
      if (languages.size() > 1)
	{
	  languages.push_back(NULL);
	  res = DropdownBox(L"Choose language", L"Please choose the patch language :", (LPCWSTR*)languages.data());
	}
      else
	res = 0;
      if (res == -1)
	return false;
      wcsncpy(lang, languages[res], 20);
      lang[20] = L'\0';
      for (LPWSTR it : languages)
	if (it != NULL)
	  free(it);
      WritePrivateProfileStringW(L"global", L"lang", lang, L"translations\\games.ini");
    }
  swprintf(this->base_path, L"%s\\translations\\%s_%s", curDir, game, lang);
  swprintf(this->ini_path, L"%s\\config.ini", this->base_path);
  return true;
}

bool	Reader::init(LPCWSTR game)
{
  if (this->_initCalled == true)
    return true;
  this->_initCalled = true;

  if (!this->getPaths(game))
    return false;
  this->readMessages();

  if (PathIsDirectoryW(L"translations") == FALSE)
    this->dispMessage(L"ERR_TRANSLATIONS_DIR_NOT_FOUND");
  else if (PathIsDirectoryW(this->base_path) == FALSE)
    this->dispMessageF(L"ERR_DIR_NOT_FOUND", this->base_path);
  else if (PathFileExistsW(this->ini_path) == FALSE)
    this->dispMessageF(L"ERR_FILE_NOT_FOUND", this->ini_path);
  else
    return true;
  return false;
}

bool	Reader::initCalled() const
{
  return this->_initCalled;
}

LPCWSTR	Reader::getFilePath(LPCWSTR file)
{
  swprintf(this->ret, L"%s\\%s", this->base_path, file);
  return this->ret;
}

LPCWSTR	Reader::iniGetString(LPCWSTR section, LPCWSTR key, LPCWSTR def)
{
  GetPrivateProfileStringW(section, key, def, this->ret, MAX_PATH, this->ini_path);
  return this->ret;
}

bool	Reader::iniGetBool(LPCWSTR section, LPCWSTR key, bool def)
{
  return GetPrivateProfileIntW(section, key, def ? 1 : 0, this->ini_path) ? true : false;
}

LPCWSTR		Reader::getNewFilesMessage()
{
  FILE*		fd;
  WCHAR*	ptr;

  fd = _wfopen(this->getFilePath(L"new_files_message.txt"), L"r, ccs=UNICODE");
  if (fd == NULL)
    return L"Some untranslated things were found while you was playing,\n"
      "but the standard message to announce it could not be found.\n"
      "Please contact the team who made this patch, send them the file\n"
      "%s ,\n"
      "and tell them they didn't include the file new_files_message.txt";
  ptr = this->ret;
  while (ptr - this->ret != 4096 && (fgetws(ptr, 4096 - (ptr - this->ret), fd)) && ptr[0] != L'\0')
    ptr += wcslen(ptr);
  fclose(fd);

  if (this->checkPercents(this->ret, {L"%s"}) == false)
    return L"Some untranslated things were found while you was playing,\n"
      "but the standard message to announce it is broken.\n"
      "Please contact the team who made this patch, send them the file\n"
      "%s ,"
      "and tell them the file new_files_message.txt must contain a \"%%s\" somewhere\n"
      "and it mustn't contain any other percent sign.";
  return this->ret;
}

Reader&	Reader::get()
{
  if (!Reader::instance)
    Reader::instance = new Reader;
  return *Reader::instance;
}
