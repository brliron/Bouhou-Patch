#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define UNICODE
#define _UNICODE
#include	<Windows.h>
#include	<stdio.h>

int		read_line(FILE* in, FILE* out, LPWSTR dest)
{
  WCHAR*	r;

  r = fgetws(dest, 65536, in);
  if (r == NULL || dest[0] == L'\0')
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

int			read_text(LPWSTR line, FILE* in, FILE* out)
{
  WCHAR			buff[65536];
  int			r;

  line[0] = L'\0';
  while ((r = read_line(in, out, buff)) == 1)
    {
      if (buff[2] != L':')
	wcscat(line, buff);
      else
	{
/*	  c = new DisplayedChar;
	  swscanf(buff, L"%*2d:%c %f %f %f %f %*d %d %d  %d %d %d %d\n", &c->c, &c->tx1, &c->tx2, &c->ty1, &c->ty2, &c->sw, &c->sh, &c->dst_x1, &c->dst_x2, &c->dst_y1, &c->dst_y2);
	  c->sy = Char::get(c->tx1, c->ty1)->sy;
	  list.push_back(c);*/
	}
    }
  if (r == 0 || line[0] != L'\0')
    return 1;
  else
    return 0;
}

void				handle_file(FILE* in, FILE* out)
{
  WCHAR				text[65536];

  while (read_text(text, in, out))
    {
      if (text[0] != L'\0')
	{
	  fputws(text, out);
	  fputws(L"\n\n", out);
	}
    }
}

int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			in;
  FILE*			out;

  // freopen("log.txt", "w, ccs=UNICODE", stdout); // Or find a way to display unicode characters in the console.
  hFindFile = FindFirstFile(L"dialogs*.txt", &data);
  do
    {
      in = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (in == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      wcscpy(data.cFileName + wcslen(data.cFileName) - 4, L"_all_chars_v2.txt");
      if ((out = _wfopen(data.cFileName, L"w, ccs=UNICODE")) == NULL)
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
