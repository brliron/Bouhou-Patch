#include	"CharDisplayer.hpp"
#include	"Output.hpp"

/*
** TODO: we should use all characters from chars.csv instead of only the ones from the string,
** so that the line spacing would not depend of the content of the string.
*/
int 	ACharDisplayer::getMaxH(LPCWSTR str)
{
  int	maxH;

  maxH = 0;
  for (int i = 0; str[i]; i++)
    {
      const Char& c = *Char::get(str[i]);
      if (c.sh > maxH)
	maxH = c.sh;
    }
  return maxH;
}

void	ACharDisplayer::display(LPCWSTR str, int x, int y, int w, int h)
{
  Char	c;
  int	maxH;
  int	curX;
  int	curY;

  maxH = this->getMaxH(str);
  curX = x;
  curY = y;
  for (int i = 0; str[i]; i++)
    {
      c = *Char::get(str[i]);
      /*
      ** TODO: put end of lines between words. The current algo put them between characters.
      ** Example of implementation: copy the string before this loop, parse it, replace some spaces
      ** with \n characters. And during this loop, when a \n is encoutered, skip it and reset x / increment y.
      */
      if (w != -1 && curX + c.sw > x + w)
	{
	  curX = 0;
	  curY += maxH + (maxH / 10 > 2 ? maxH / 10 : 2); // TODO: this line probably gives a nice value for the new baseline, but I should ensure it does.
	  if (h != -1 && curY + maxH > y)
	    Output::printf(L"In ACharDisplayer::display(LPCWSTR, %d, %d, %d, %d), The line %s overflowed.\n", x, y, w, h, str);
	}
      this->ATextDisplayer::display(c, curX, curY); // I think I shouldn't have to explicitely tell I want to call the function from the base class. TODO: check if I did something wrong.
      curX += c.sw;
    }
}

void	ACharDisplayer::display(const std::list<DisplayedChar>& list)
{
  for (const DisplayedChar& c : list)
    this->display(c);
}
