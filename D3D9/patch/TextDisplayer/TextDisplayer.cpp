#include	"TextDisplayer.hpp"

void	ATextDisplayer::display(const Char& c, int x, int y)
{
  this->display(c, x, x + c.sw, y + c.sy, y + c.sy + c.sh);
}

void		ATextDisplayer::display(const Char& c, int x1, int x2, int y1, int y2)
{
  DisplayedChar	d;

  d = c;
  d.dst_x1 = x1;
  d.dst_x2 = x2;
  d.dst_y1 = y1;
  d.dst_y2 = y2;
  this->display(d);
}
