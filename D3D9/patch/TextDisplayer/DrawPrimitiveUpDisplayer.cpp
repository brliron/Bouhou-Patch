#include	"DrawPrimitiveUpDisplayer.hpp"

DrawPrimitiveUpDisplayer::DrawPrimitiveUpDisplayer(D3DDevice9* d3dd)
  : d3dd(d3dd)
{}

DrawPrimitiveUpDisplayer::~DrawPrimitiveUpDisplayer()
{}

void	DrawPrimitiveUpDisplayer::fill_vertex_tab(TLVERTEX tab[4], int x1, int x2, int y1, int y2, const Char& c)
{
  for (int i = 0; i < 4; i++)
    {
      tab[i].z = 0;
      tab[i].rhw = 1.0f;
      tab[i].colour = 0xFFFFFFFF;
    }

  tab[0].x = x1 - 0.5;
  tab[1].x = x2 - 0.5;
  tab[2].x = x1 - 0.5;
  tab[3].x = x2 - 0.5;
  tab[0].y = y1 - 0.5;
  tab[1].y = y1 - 0.5;
  tab[2].y = y2 - 0.5;
  tab[3].y = y2 - 0.5;
  tab[0].u = c.tx1;
  tab[1].u = c.tx2;
  tab[2].u = c.tx1;
  tab[3].u = c.tx2;
  tab[0].v = c.ty1;
  tab[1].v = c.ty1;
  tab[2].v = c.ty2;
  tab[3].v = c.ty2;
}

void	DrawPrimitiveUpDisplayer::display(const DisplayedChar& c)
{
  TLVERTEX	tab[4];

  this->fill_vertex_tab(tab, c.dst_x1, c.dst_x2, c.dst_y1, c.dst_y2, c);
#ifndef DISABLE_DRAW_PRIMITIVE_UP
  this->d3dd->::D3DDevice9::DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, tab, 28);
#endif /* !DISABLE_DRAW_PRIMITIVE_UP */
}
