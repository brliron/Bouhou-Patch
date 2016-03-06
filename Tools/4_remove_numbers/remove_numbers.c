#define	UNICODE
#define	_UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	<string.h>

int		delete_line(LPCWSTR dest)
{
  LPWSTR	ptr;

  if (!(L'0' <= dest[0] && dest[0] <= L'9'))
    return 0;
  wcstol(dest, &ptr, 10);
  if (ptr != dest && (*ptr == L'\n' || *ptr == L'\0'))
    return 1;
  return 0;
}

int		read_line(FILE* in, LPWSTR dest)
{
  static int	cur_line = 0;

  if (fgetws(dest, 65535, in) == NULL)
    return -1;
  cur_line++;
  if (delete_line(dest))
    {
      while (fgetws(dest, 65535, in) != NULL)
        {
	  cur_line++;
	  if (dest[0] != L'\n')
	    return 0;
	}
      return -1;
    }
  return 0;
}

void		handle_file(FILE* in, FILE* out)
{
  wchar_t	cur_line[65536];

  while (read_line(in, cur_line) == 0)
    fwprintf(out, L"%s", cur_line);
}

int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			in;
  FILE*			out;

  hFindFile = FindFirstFile(L"dialogs*_all_chars_v2.txt", &data);
  do
    {
      in = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (in == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      out = _wfopen(L"out.txt", L"w, ccs=UNICODE");
      if (out == NULL)
	{
	  _wperror(L"out.txt");
	  continue ;
	}
      handle_file(in, out);
      fclose(in);
      fclose(out);
      CopyFile(L"out.txt", data.cFileName, FALSE);
    } while (FindNextFile(hFindFile, &data));
  FindClose(hFindFile);
  DeleteFile(L"out.txt");
  return 0;
}
