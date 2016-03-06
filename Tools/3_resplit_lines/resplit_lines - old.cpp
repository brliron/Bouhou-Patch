#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define UNICODE
#define _UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	"BohoCharBuff.hpp"
#include	"Log.hpp"

extern Char	boho1_chars[];
extern Char*	chars;

int		read_line(FILE* in, LPWSTR dest)
{
  WCHAR*	r;

  r = fgetws(dest, 65536, in);
  if (r == NULL || dest[0] == L'\0')
    return -1;
  if (dest[0] == L'#')
    {
      my_log->put_text(dest);
      // fwprintf(out, L"%s", dest);
      return read_line(in, dest);
    }
  if (dest[0] == L'\n')
    {
      // fwprintf(out, L"\n");
      return 0;
    }
  return 1;
}

bool			read_text(LPWSTR line, std::list<DisplayedChar*>& list, FILE* in)
{
  DisplayedChar*	c;
  WCHAR			buff[65536];
  int			r;

  line[0] = L'\0';
  list.clear();
  while ((r = read_line(in, buff)) == 1)
    {
      if (buff[2] != L':')
	wcscat(line, buff);
      else
	{
	  c = new DisplayedChar;
	  swscanf(buff, L"%*2d:%c %f %f %f %f %*d %d %d  %d %d %d %d\n", &c->c, &c->tx1, &c->tx2, &c->ty1, &c->ty2, &c->sw, &c->sh, &c->dst_x1, &c->dst_x2, &c->dst_y1, &c->dst_y2);
	  if (Char::get(c->tx1, c->ty1))
	    c->sy = Char::get(c->tx1, c->ty1)->sy;
	  else
	    {
	      c->c = L'\0';
	      c->log(0);
	    }
	  list.push_back(c);
	}
    }
  if (r == 0 || list.empty() == false)
    return true;
  else
    return false;
}

void				handle_file(FILE* in)
{
  WCHAR				text[65536];
  std::list<DisplayedChar*>	list;
  ACharBuff*			charbuff;

  while (read_text(text, list, in))
    {
      if (list.empty())
	continue ;
      charbuff = new BohoCharBuff(list, NULL, CHARBUFF__DISABLE_REPLACE_STR);
      charbuff->ACharBuff::dump_str();
      delete charbuff;
      list.clear();
    }
}

void	init()
{
  chars = boho1_chars;
  my_log = new Log(Log::HIDE_KNOWN_TEXTS);
  // my_log = new Log(LOG__PRINT_ALL_CHARS);
}

int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			in;

  init();
  // freopen("log.txt", "w, ccs=UNICODE", stdout); // Or find a way to display unicode characters in the console.
  hFindFile = FindFirstFile(L"dialogs*_all_chars.txt", &data);
  // hFindFile = FindFirstFile(L"dialogs_003_all_chars.txt", &data);
  do
    {
      in = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (in == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      wcscpy(data.cFileName + wcslen(data.cFileName) - 4, L"_v2.txt");
      if (my_log->open_file(data.cFileName) == false)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      handle_file(in);
      fclose(in);
    } while (FindNextFile(hFindFile, &data));
  FindClose(hFindFile);
  delete my_log;
  return 0;
}
