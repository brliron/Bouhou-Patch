#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define UNICODE
#define _UNICODE
#include	<Windows.h>
#include	<stdio.h>
#include	"chars.hpp"

bool	init();

bool			complete_char_from_dialogs(Char* c)
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  FILE*			fd;
  WCHAR			buff[65536];
  float			x;
  float			y;

  hFindFile = FindFirstFile(L"dialogs*.txt", &data);
  do
    {
      fd = _wfopen(data.cFileName, L"r, ccs=UNICODE");
      if (fd == NULL)
	{
	  _wperror(data.cFileName);
	  continue ;
	}
      while (fgetws(buff, 65536, fd))
	{
	  if (buff[2] != L':')
	    continue ;
	  x = _wtof(buff + 5);
	  y = _wtof(buff + 23);
	  if (float_cmp(c->tx1, x) == false || float_cmp(c->ty1, y) == false)
	    continue ;
	  swscanf(buff, L"%*2d:%*c %f %f %f %f %d %d %d", &c->tx1, &c->tx2, &c->ty1, &c->ty2, &c->sy, &c->sw, &c->sh);
	  fclose(fd);
	  FindClose(hFindFile);
	  return true;
	}
      fclose(fd);
    } while (FindNextFile(hFindFile, &data));
  FindClose(hFindFile);
  fwprintf(stderr, L"Warning : could not find %c in dialog files.\n", c->c);
  return false;
}

void	log_character(LPCWSTR filename)
{
  Char	c;
  int	x;
  int	y;

  swscanf(filename, L"%c_%d_%d.bmp", &c.c, &x, &y);
  c.tx1 = x / 10000.0;
  c.ty1 = y / 10000.0;
  if (complete_char_from_dialogs(&c))
    c.log(CHARBUFF__NO_FLAGS);
}

int			main()
{
  HANDLE		hFindFile;
  WIN32_FIND_DATA	data;
  int			i;

  if (init() == false)
    return 1;
  i = 1;
  hFindFile = FindFirstFile(L"characters\\*.bmp", &data);
  do
    {
      if (data.cFileName[0] == L'-')
	continue ;
      if (data.cFileName[1] != L'_')
	{
	  fwprintf(stderr, L"Warning : %s : multiple characters given. Ignoring.\n", data.cFileName);
	  continue ;
	}
      log_character(data.cFileName);
      wprintf(L"%d\n", i);
      i++;
    } while (FindNextFile(hFindFile, &data));
  FindClose(hFindFile);
  return 0;
}
