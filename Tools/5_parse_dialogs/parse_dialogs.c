#define	UNICODE
#define	_UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	<string.h>

/*
** When this define is set to 1, The text buffer is dumped just before any comment.
** When it is set to 0, the "Loading..." comments are written without dumping the text buffer.
** The last one may be better if the "Loading..." message is displayed during a dialog. Or not.
*/
#define	WRITE_COMMENT_NOW	0

LPCWSTR	replace_str[] =
{
  L"#  An unknown character is talking (u=0.378906, v=0.882813)\n",	L"#  Meiling :\n",
  L"#  Patchouli is talking (u=0.503906, v=0.882813)\n",		L"#  Patchouli :\n",
  L"#  Sakuya is talking (u=0.628906, v=0.882813)\n",			L"#  Sakuya :\n",
  L"#  An unknown character is talking (u=0.753906, v=0.882813)\n",	L"#  Remilia :\n",
  L"#  Flandre is talking (u=0.878906, v=0.882813)\n",			L"#  Flandre :\n",
  NULL
};

FILE*	g_out = NULL;
void	save_old_line(FILE* out, wchar_t* cur_line);

void	write_comment(LPCWSTR str)
{
  static WCHAR	buff[65535] = L"\0";

  if (str == NULL && buff[0])
    {
      fwprintf(g_out, L"%s", buff);
      buff[0] = L'\0';
    }
  else if (str)
    wcscat(buff, str);
}

void	replace_comment(LPWSTR str)
{
  int	i;

  i = 0;
  while (replace_str[i] && wcscmp(replace_str[i], str) != 0)
    i += 2;
  if (replace_str[i])
    wcscpy(str, replace_str[i + 1]);
}

int	read_line(FILE* in, wchar_t* dest, int* cur_line)
{
  if (fgetws(dest, 65535, in) == NULL)
    return -1;
  (*cur_line)++;
  if (dest[0] == L'\n')
    return 0;
  if (dest[0] == L'#' || dest[0] == L'\n')
    {
      if (dest[0] == L'#')
	{
	  // if (WRITE_COMMENT_NOW || dest[1] != L'#')
	    // save_old_line(g_out, L"");
	  replace_comment(dest);
	  // fwprintf(g_out, L"%s", dest);
	  write_comment(dest);
	}
      return read_line(in, dest, cur_line);
    }
  if (dest[wcslen(dest) - 1] == L'\n')
    dest[wcslen(dest) - 1] = L'\0';
  // wchar_t	buff[65536];
  // if (fgetws(buff, 65535, in) == NULL)
    // {
      // if (dest[0] != L'\0')
	// printf("Line %d : error : missing translation.\n", *cur_line);
      // else
	// printf("Line %d : error : this line should be empty.\n", *cur_line);
      // return -2;
    // }
  // (*cur_line)++;
  if (dest[0])
    return 1;
  else
    return 0;
}

int		read_text(FILE* in, wchar_t* dest)
{
  static int	cur_line = 1;
  int		i;

  dest[0] = L'\0';
  while ((i = read_line(in, dest + wcslen(dest), &cur_line)) < 0 || i == 1)
    {
      if (i == -1)
	printf("%d lines read.\n", cur_line - 1);
      if (i < 0)
	return 0;
      wcscat(dest, L"\n");
    }
  while (dest[wcslen(dest) - 1] == L'\n')
    dest[wcslen(dest) - 1] = L'\0';
  return 1;
}

void		save_old_line(FILE* out, wchar_t* cur_line)
{
  static wchar_t	old_line[65536];

  if (cur_line == NULL)
    old_line[0] = L'\0';
  if (old_line[0] == L'\0')
    {
      if (cur_line)
	wcscpy(old_line, cur_line);
      return ;
    }
  if (wcsncmp(old_line, cur_line, wcslen(old_line)) == 0)
    {
      write_comment(NULL); // If cur_line is the continuation of old_line, the comments should be printed before the line.
      wcscpy(old_line, cur_line);
      return ;
    }

  fwprintf(out, L"%s\n\n", old_line);
  wcscpy(old_line, cur_line);
  write_comment(NULL); // If cur_line is a new line, the comments should be printed after old_line.
}

void		handle_file(FILE* in, FILE* out)
{
  wchar_t	cur_line[65536];

  save_old_line(NULL, NULL);
  while (read_text(in, cur_line))
    {
      if (cur_line[0] == L'\0')
	continue ;
      save_old_line(out, cur_line);
    }
  save_old_line(out, cur_line);
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
      wcscpy(data.cFileName + wcslen(data.cFileName) - 4, L" - jp sentences.txt");
      out = _wfopen(data.cFileName, L"w, ccs=UNICODE");
      g_out = out; // Because I'm too lazy to give this variable to read_line.
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
