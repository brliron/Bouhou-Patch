#ifndef ARCHIVE_HPP_
# define ARCHIVE_HPP_

# include       <map>
# include       <string>
# include       <stdio.h>
# include       "archive.h"
# include       "archive_entry.h"

/*
** This class is an encapsulation of the libarchive.
** For now, it only supports the tar archive format.
** Important note : the file operations are done when the corresponding function is called
** (for example, in a call to addFile, the given file is read during the function call),
** but the archive is written only when you call the write member function.
** So, if you call addFile and set the deleteOnDisk parameter to true, then exit the program without
** calling write, the file will be deleted from disk, but it won't be written to the archive. It will be lost.
*/
class   Archive
{
private:
  struct                File
  {
    BYTE*               data;
    unsigned long       size;
  };

  std::wstring                  filename;
  std::map<std::wstring, File>  files;

  bool                          _read_openFiles(struct archive*& in, BYTE*& file_buffer) const;
  std::wstring                  _formatFilename(const std::wstring& in) const;
  void                          _createPathForFile(const std::wstring& filename) const;

  FILE*                         _open(struct archive*& archive) const;
  void                          _writeFile(struct archive* archive, const std::wstring& filename, const File& file) const;
  void                          _close(FILE* stream, struct archive* archive) const;

public:
  Archive();
  ~Archive();

  // Opens an archive. Calls read if the archive exists, create otherwise.
  bool  open(LPCWSTR filename);
  // Creates an archive. As the archive is written only when the write function is called, this function only does some initialisation.
  void  create(LPCWSTR filename);
  // Opens an archive, and loads its content in memory.
  bool  read(LPCWSTR filename);

  // Adds a file to an archive. The 2nd prototype reads the file from the disk.
  void  addFile(const std::wstring& filename, BYTE* data, unsigned long size);
  bool  addFile(const std::wstring& filename, bool deleteOnDisk = false);
  // Stores on the disk a file from the archive.
  bool  extractFile(const std::wstring& filename, bool deleteFromArchive = false);
  // Remove a file from the archive.
  bool  removeFile(const std::wstring& filename);
  // Writes the archive on the disk. Note that the Archive instance is unchanged after calling this function.
  void  write() const;
  // Closes the archive.
  void  close();

  // Returns true if the archive contains a file with the given filename, false otherwise.
  bool  hasFile(LPCWSTR filename) const;
};

#endif /* !ARCHIVE_HPP_ */
