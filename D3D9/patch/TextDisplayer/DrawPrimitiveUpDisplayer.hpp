#ifndef DRAWPRIMITIVEUPDISPLAYER_HPP_
# define DRAWPRIMITIVEUPDISPLAYER_HPP_

# include	"engine/D3DDevice9.hpp"
# include	"CharDisplayer.hpp"

/*
** Implementation of the character-based displayer using DrawPrimitiveUp from Direct3D 9.
*/
class	DrawPrimitiveUpDisplayer : public ACharDisplayer
{
private:
  D3DDevice9*	d3dd;

  void	fill_vertex_tab(TLVERTEX tab[4], int x1, int x2, int y1, int y2, const Char& c);

public:
  DrawPrimitiveUpDisplayer(D3DDevice9* d3dd);
  ~DrawPrimitiveUpDisplayer();

  void	display(const DisplayedChar& c);
};

#endif /* !DRAWPRIMITIVEUPDISPLAYER_HPP_ */
