#ifndef BOHO_CHARBUFF_HPP_
# define BOHO_CHARBUFF_HPP_

# include	"chars.hpp"
# include	"Boho/D3DDevice9.hpp"

/*
** Subclass of ACharBuff for the Boho games.
*/
namespace Boho
{
  class	CharBuff : public ACharBuff
  {
  private :
    Boho::AD3DDevice9*		d3dd;

    void	put_vertex_tab(TLVERTEX tab[4]);
    void	put_char(const Char& c, int x1, int x2, int y1, int y2);
    void	put_char(const Char& c, int x, int y);
    void	put_char(const DisplayedChar& c);

  public:
    CharBuff(Boho::AD3DDevice9* d3dd);
    CharBuff(std::list<DisplayedChar*>& buff, Boho::AD3DDevice9* d3dd, int flags = CHARBUFF__NO_FLAGS);
    ~CharBuff();

    void	dump_str();
  };
}

#endif /* !BOHO_CHARBUFF_HPP_ */
