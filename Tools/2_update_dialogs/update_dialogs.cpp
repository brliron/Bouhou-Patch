#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define UNICODE
#define _UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	"chars.hpp"

#define OUTPUT_CHARS_LOG

WCHAR		cur_filename[MAX_PATH];

int		read_line(FILE* in, FILE* out, LPWSTR dest)
{
  WCHAR*	r;

  r = fgetws(dest, 65536, in);
  if (r == NULL)
    dest[0] = L'\0';
  if (dest[0] == L'\0')
    return -1;
  if (dest[0] == L'#')
    {
      fwprintf(out, L"%s", dest);
      return read_line(in, out, dest);
    }
  if (dest[0] == L'\n')
    {
      // fwprintf(out, L"\n");
      return 0;
    }
  return 1;
}

void		handle_file(FILE* in, FILE* out)
{
  LPWSTR	text[200];
  int		n;
  float		x;
  float		y;
  bool		read_from_text0;

  for (int i = 0; i < 200; i++)
    text[i] = (LPWSTR)malloc(65536);
  read_from_text0 = false;
  while (read_from_text0 || read_line(in, out, text[0]) != -1)
    {
      read_from_text0 = false;
      if (text[0][0] == L'\n')
	{
	  fwprintf(out, L"\n");
	  continue ;
	}
      n = 0;
      while (read_line(in, out, text[n + 1]) > 0)
	{
	  if (text[n + 1][2] != L':')
	    {
	      read_from_text0 = true;
	      break ;
	    }
	  if (text[0][n] != L'?')
	    {
	      n++;
	      continue ;
	    }
	  swscanf(text[n + 1], L"%*2d:%*c %f %*f %f %*f", &x, &y);
	  if (Char::get(x, y) == NULL)
	    {
	      wprintf(L"Warning in %s : %sCound not find %dth character\n", cur_filename, text[0], n);
	      n++;
	      continue ;
	    }
	  text[0][n] = Char::get(x, y)->c;
	  text[n + 1][3] = Char::get(x, y)->c;
	  n++;
	}
      fwprintf(out, L"%s", text[0]);
#ifdef OUTPUT_CHARS_LOG
      for (int i = 0; i < n; i++)
	fwprintf(out, L"%s", text[i + 1]);
#endif /* OUTPUT_CHARS_LOG */
      if (read_from_text0)
	wcscpy(text[0], text[n + 1]);
      else if (text[n + 1][0] == L'\n')
	fwprintf(out, L"\n");
    }
  for (int i = 0; i < 200; i++)
    free(text[i]);
}

bool	init();
int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			in;
  FILE*			out;

  if (init() == false)
    {
      fprintf(stderr, "Init failed.\n");
      return 1;
    }
  // freopen("log.txt", "w, ccs=UNICODE", stdout); // Or find a way to display unicode characters in the console.
  hFindFile = FindFirstFile(L"dialogs*.txt", &data);
  do
    {
      wcscpy(cur_filename, data.cFileName);
      in = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (in == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      wcscpy(data.cFileName + wcslen(data.cFileName) - 4, L"_all_chars.txt");
      out = _wfopen(data.cFileName, L"w, ccs=UNICODE");
      if (out == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      handle_file(in, out);
      fclose(in);
      fclose(out);
    } while (FindNextFile(hFindFile, &data));
  FindClose(hFindFile);
  return 0;
}
