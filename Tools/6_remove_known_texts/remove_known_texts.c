#define	UNICODE
#define	_UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	<string.h>

#define	LINE_SIZE	1000
#define	MIN(x, y)	((x) <= (y) ? (x) : (y))

LPWSTR	texts_to_remove[100000];/* = 
{
  L"CAKE_L\n"
   "ごほうびサイズの大きなケーキ。食ベると体カを大き\n"
   "く回復します。",
  L"GLOVE\n"
   "美鈴專用装備。装備すると、特定の障害物を破\n"
   "壞する事ができます。",
  NULL
};*/

int	read_text(FILE* in, LPWSTR dest)
{
  static wchar_t	last_text[LINE_SIZE] = L"";
  wchar_t		buff[LINE_SIZE];

  dest[0] = L'\0';
  if (last_text[0] != L'\0')
    {
      wcscpy(dest, last_text);
      last_text[0] = L'\0';
    }
  if (dest[0] == L'#')
    return 0;
  buff[0] = L'\0';
  while (fgetws(buff, LINE_SIZE, in) != NULL && buff[0] != L'\n' && buff[0] != L'\0' && buff[0] != L'#')
    {
      wcscat(dest, buff);
      buff[0] = L'\0';
    }
  if (buff[0] == L'#')
    {
      if (dest[0])
	wcscpy(last_text, buff);
      else
	wcscpy(dest, buff);
      return 0;
    }
  if (buff[0] != L'\n') // buff[0] == L'\0' or fgetws returned NULL
    {
      if (dest[0] != L'\0')
	return 0;
      else
	return -1;
    }
  wcscat(dest, buff);
  buff[0] = L'\0';
  while (fgetws(buff, LINE_SIZE, in) != NULL && buff[0] == L'\n' && buff[0] == L'\0')
    {
      wcscat(dest, buff);
      buff[0] = L'\0';
    }
  wcscpy(last_text, buff); // If there is nothing to copy, buff[0] should be L'\0'.
  return 0;
}

int		remove_text(LPCWSTR text)
{
  unsigned int	len;
  int		i;

  len = wcslen(text);
  while (len && text[len - 1] == L'\n')
    len--;
  i = 0;
  while (texts_to_remove[i])
    {
      if (wcsncmp(text, texts_to_remove[i], MIN(len, wcslen(texts_to_remove[i]))) == 0)
        return 1;
      i++;
    }
  return 0;
}

void		handle_file(FILE* in, FILE* out)
{
  wchar_t**	file;
  int		n;

  file = malloc(100000 * sizeof(wchar_t*));
  for (n = 0; n < 100000; n++)
    file[n] = malloc(LINE_SIZE * sizeof(wchar_t));

// Remove unwanted texts
  n = 0;
  while (read_text(in, file[n]) != -1)
    if (remove_text(file[n]) == 0)
      n++;
  file[n][0] = L'\0'; // I don't set it to NULL because I don't want to lose all the space allocated before (I don't care a lot about memory management for these scripts, but I care at least a bit).

// Remove consecutive stage numbers
  n = 0;
  while (file[n][0])
    {
      if (n == 0 || file[n][0] != L'#' || file[n][1] != L'#' || file[n - 1][0] != L'#' || file[n - 1][1] != L'#')
        fwprintf(out, L"%s", file[n]);
      n++;
    }

  for (n = 0; n < 100000; n++)
    free(file[n]);
  free(file);
}

void		add_text_to_remove(LPWSTR str)
{
  static int	n = 0;
  int		i;

  i = wcslen(str) - 1;
  while (i && str[i] == L'\n')
    i--;
  str[i + 1] = L'\0';
  texts_to_remove[n] = _wcsdup(str);
  texts_to_remove[n + 1] = NULL;
  n++;
}

void		init_texts_to_remove()
{
  LPWSTR	files_list[] =
  {
    L"dialogs - intro - jp sentences.txt",
    L"dialogs - items - jp sentences.txt",
    L"dialogs - map texts.txt",
    L"dialogs - LIFE - MAG - STR - RNG - JUMP - GOLD.txt",
    L"endings.txt",
    NULL
  };
  int		i;
  FILE*		fd;
  wchar_t	buff[LINE_SIZE];

  i = 0;
  while (files_list[i])
    {
      fd = _wfopen(files_list[i], L"r, ccs=UNICODE");
      if (fd == NULL)
	{
	  _wperror(files_list[i]);
	  continue ;
	}
      while (read_text(fd, buff) != -1)
        add_text_to_remove(buff);
      i++;
      fclose(fd);
    }
}

int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			in;
  FILE*			out;

  init_texts_to_remove();
  hFindFile = FindFirstFile(L"dialogs*_all_chars_v2 - jp sentences.txt", &data);
  do
    {
      in = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (in == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      wcscpy(data.cFileName + wcslen(data.cFileName) - 32, L" - jp sentences.txt");
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
