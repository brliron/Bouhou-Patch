#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif
#include	<stdlib.h>
#include	<string.h>
#include	"chars.hpp"

struct	Entry
{
  int	begin;
  int	val;
  int	n1;
  int	n2;
  int	n3;
};

#define	NONE	0
#define	LEFT	1
#define	CENTER	2

static int	get_num(LPCWSTR* in, int def)
{
  LPCWSTR	str;

  str = wcschr(*in, L';');
  if (str == NULL)
    return def;
  str++;
  *in = str;
  if (_wtoi(str) >= 0)
    return _wtoi(str);
  else
    return def;
}

static void	init(const std::vector<LPCWSTR> strings, Entry indexes[32])
{
  LPCWSTR	str;
  bool		is_init = false;
  int		j;

  if (is_init)
    return ;
  j = 0;
  for (int i = 0; strings[i]; i += 2)
    {
      if (strings[i][0] != L'#')
	continue ;
      indexes[j].begin = i;
      if (wcscmp(strings[i], L"#none") == 0)
	indexes[j].val = NONE;
      else if (wcsncmp(strings[i], L"#left", 5) == 0)
	{
	  indexes[j].val = LEFT;
	  str = strings[i];
	  indexes[j].n1 = get_num(&str, 50);
	  indexes[j].n2 = get_num(&str, INT_MAX);
	  indexes[j].n3 = get_num(&str, 20);
	}
      else if (wcsncmp(strings[i], L"#center", 7) == 0)
	{
	  indexes[j].val = CENTER;
	  str = strings[i];
	  indexes[j].n1 = get_num(&str, 0);
	  indexes[j].n2 = get_num(&str, 640);
	}
      else
	continue ;
      j++;
    }
  indexes[j].begin = -1;
  is_init = true;
}

/*
** Align the text to the left, and automatically goes to a new line when the text reaches the given point.
**
** Example of use :
** #left;275;615;20
** 275 is dst_x1 for the 1st character (-1 to keep the original one).
** 615 is the point where it will return to the new line (-1 or a really big number to never do this).
** 20 is a line's size (-1 to use a default value (20)).
*/
static int				left(Entry& entry, std::list<DisplayedChar*>::iterator& begin, std::list<DisplayedChar*>::iterator& end)
{
  std::list<DisplayedChar*>::iterator	it = begin;
  int					base_x;
  int					x;
  int					y_shift;
  int					nb_lines;

  if (entry.n1 >= 0)
    base_x = entry.n1;
  else
    base_x = (*begin)->dst_x1;
  x = base_x;
  y_shift = 0;
  nb_lines = 1;
  while (it != end)
    {
      (*it)->dst_x1 = x;
      (*it)->dst_x2 = x + (*it)->sw;
      (*it)->dst_y1 += y_shift;
      (*it)->dst_y2 += y_shift;
      x += (*it)->sw;
      if (x > entry.n2)
	{
	  while (it != begin && (*it)->c != L' ')
	    {
	      (*it)->dst_y1 -= y_shift; // Reverse the "+= y_shift" some lines above.
	      (*it)->dst_y2 -= y_shift;
	      --it;
	    }
	  y_shift += entry.n3;
	  x = base_x;
	  nb_lines++;
	}
      ++it;
    }
  return nb_lines;
}

/*
** Center the text on the screen.
**
** Example of use :
** #center;0;640
** 0 is the left side.
** 640 is the right side.
*/
static int				center(Entry& entry, std::list<DisplayedChar*>::iterator& begin, std::list<DisplayedChar*>::iterator& end)
{
  std::list<DisplayedChar*>::iterator	it = begin;
  int					w;
  int					screen_l;
  int					screen_r;
  int					x;

  w = 0;
  while (it != end)
    {
      w += (*it)->sw;
      ++it;
    }

  screen_l = entry.n1;
  screen_r = entry.n2;
  x = (screen_r - screen_l) / 2 - w / 2 + screen_l;
  it = begin;
  while (it != end)
    {
      (*it)->dst_x1 = x;
      (*it)->dst_x2 = x + (*it)->sw;
      x += (*it)->sw;
      ++it;
    }
  return screen_r - screen_l > w ? 0 : 1;
}

int		move_texts(const std::vector<LPCWSTR>& strings, int idx, std::list<DisplayedChar*>::iterator begin, std::list<DisplayedChar*>::iterator end)
{
  static Entry	indexes[32];
  int		i;

  init(strings, indexes);
  for (i = 0; indexes[i].begin != -1 && idx > indexes[i].begin; i++);
  i--;
  if (i == -1)
    return 0;
  if (indexes[i].val == LEFT)
    return left(indexes[i], begin, end);
  else if (indexes[i].val == CENTER)
    return center(indexes[i], begin, end);
  else
    return 0;
}
