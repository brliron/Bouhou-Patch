#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	<Shlwapi.h>
#include	"Archive.hpp"

Archive::Archive()
{}

Archive::~Archive()
{
  if (this->filename != L"")
    this->close();
}


static BYTE*	readFile(const std::wstring& filename, unsigned long& fileSize)
{
  HANDLE	hFile;
  BYTE*		buffer;

  hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return NULL;
  fileSize = GetFileSize(hFile, NULL);
  buffer = new BYTE[fileSize];
  if (ReadFile(hFile, buffer, fileSize, &fileSize, NULL) == 0)
    {
      delete[] buffer;
      buffer = NULL;
    }
  CloseHandle(hFile);
  return buffer;
}





bool		Archive::_read_openFiles(struct archive*& in, BYTE*& file_buffer) const
{
  DWORD		file_size;

  file_buffer = readFile(this->filename, file_size);
  if (file_buffer == NULL)
    return false;
  in = archive_read_new();
  archive_read_support_format_tar(in);
  archive_read_open_memory(in, file_buffer, file_size);
  return true;
}

bool			Archive::read(LPCWSTR filename)
{
  struct archive*	in;
  BYTE*			in_buffer;
  struct archive_entry*	entry;
  int			ret;
  BYTE*			buff;
  int			entry_size;

  if (!this->filename.empty())
    return true;

  this->filename = filename;
  if (this->_read_openFiles(in, in_buffer) == false)
    {
      this->filename = L"";
      return false;
    }

  buff = NULL;
  while ((ret = archive_read_next_header(in, &entry)) == ARCHIVE_OK)
    {
      entry_size = archive_entry_size(entry);
      buff = new BYTE[entry_size];
      archive_read_data(in, buff, entry_size);
      this->addFile(archive_entry_pathname_w(entry), buff, entry_size);
    }

  archive_read_close(in);
  archive_read_free(in);
  delete[] in_buffer;

  if (ret != ARCHIVE_EOF)
    return false;
  return true;
}

void	Archive::create(LPCWSTR filename)
{
  if (this->filename.empty())
    this->filename = filename;
}

bool	Archive::open(LPCWSTR filename)
{
  if (!this->filename.empty())
    return true;

  if (PathFileExistsW(filename))
    return this->read(filename);
  else
    {
      this->create(filename);
      return true;
    }
}





std::wstring	Archive::_formatFilename(const std::wstring& in) const
{
  std::wstring	out;

  out = in;
  while (out.find(L'\\') != std::wstring::npos)
    out[out.find(L'\\')] = L'/';
  return out;
}

void	Archive::addFile(const std::wstring& filename, BYTE* data, unsigned long size)
{
  const std::wstring& name = this->_formatFilename(filename);
  auto	it = this->files.find(name);

  if (it != this->files.end())
    delete[] it->second.data;

  File	file;

  file.data = data;
  file.size = size;
  this->files[name] = file;
}

bool		Archive::addFile(const std::wstring& filename, bool deleteOnDisk)
{
  BYTE*		data;
  unsigned long	size;

  data = readFile(filename, size);
  if (data == NULL)
    return false;
  this->addFile(filename, data, size);

  if (deleteOnDisk)
    {
      WCHAR	path[MAX_PATH];
      wcscpy(path, filename.c_str());
      DeleteFileW(path);
      while (wcsrchr(path, L'/'))
	{
	  *wcsrchr(path, L'/') = L'\0';
	  if (PathIsDirectoryEmptyW(path))
	    RemoveDirectoryW(path);
	  else
	    break ;
	}
    }
  return true;
}

void		Archive::_createPathForFile(const std::wstring& filename) const
{
  WCHAR		buff[MAX_PATH];
  int		i;

  i = 0;
  for (wchar_t c : filename)
    {
      if (c == L'/' || c == L'\\')
	{
	  if (PathIsDirectoryW(buff) == FALSE)
	    CreateDirectoryW(buff, NULL);
	  buff[i] = L'\\';
	}
      else
	buff[i] = c;
      i++;
    }
}

bool		Archive::extractFile(const std::wstring& filename, bool deleteFromArchive)
{
  auto		it = this->files.find(this->_formatFilename(filename));
  FILE*		stream;

  if (it == this->files.end())
    return false;
  stream = _wfopen(filename.c_str(), L"wb");
  if (stream == NULL)
    return false;
  fwrite(it->second.data, it->second.size, 1, stream);
  fclose(stream);

  if (deleteFromArchive)
    {
      delete[] it->second.data;
      this->files.erase(it);
    }
  return true;
}

bool		Archive::removeFile(const std::wstring& filename)
{
  std::wstring	str = this->_formatFilename(filename);
  auto		it = this->files.find(str);

  if (it == this->files.end())
    return false;
  delete[] it->second.data;
  this->files.erase(it);
  return true;
}





FILE*	Archive::_open(struct archive*& archive) const
{
  FILE*	stream;

  stream = _wfopen(this->filename.c_str(), L"wb");
  if (stream == NULL)
    return NULL;
  archive = archive_write_new();
  archive_write_set_format_pax_restricted(archive);
  archive_write_open_FILE(archive, stream);
  return stream;
}

void	Archive::_writeFile(struct archive* archive, const std::wstring& filename, const File& file) const
{
  struct archive_entry*	entry;
  unsigned int		pos;
  int			r;

  entry = archive_entry_new();
  archive_entry_copy_pathname_w(entry, filename.c_str());
  archive_entry_set_size(entry, file.size);
  archive_entry_set_filetype(entry, AE_IFREG);
  archive_entry_set_perm(entry, 0644);
  archive_write_header(archive, entry);
  pos = 0;
  while (pos < file.size)
    {
      r = archive_write_data(archive, file.data + pos, file.size - pos);
      if (r == -1)
	break ;
      pos += r;
    }
  archive_entry_free(entry);
}

void	Archive::_close(FILE* stream, struct archive* archive) const
{
  archive_write_close(archive);
  archive_write_free(archive);
  fclose(stream);
}

void			Archive::write() const
{
  FILE*			stream;
  struct archive*	archive;

  if (this->files.size() == 0)
    {
      if (PathFileExistsW(this->filename.c_str()))
	DeleteFileW(this->filename.c_str());
      return ;
    }

  stream = this->_open(archive);
  if (stream == NULL)
    return ;
  for (auto& it : this->files)
    this->_writeFile(archive, it.first, it.second);
  this->_close(stream, archive);
}


void	Archive::close()
{
  for (auto& it : this->files)
    delete[] it.second.data;
  this->files.clear();
  this->filename = L"";
}





bool		Archive::hasFile(LPCWSTR filename) const
{
  std::wstring	str = this->_formatFilename(filename);

  if (this->files.count(str) == 1)
    return true;
  return false;
}
