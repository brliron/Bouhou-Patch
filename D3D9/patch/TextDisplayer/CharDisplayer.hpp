#ifndef CHARDISPLAYER_HPP_
# define CHARDISPLAYER_HPP_

# include	"TextDisplayer.hpp"

/*
** Interface for the character-based displayers. Usually based on a bitmap font.
** The functions taking a string or a list are implemented by calling
** display(const DicplayedChar&) for each character. Implementations only need
** to implement this function.
**
** Note: this displayer uses Char::get to create a DisplayedChar when needed,
** so a chars.csv file must be present and filled correctly. Else, Char::get
** will return a null reference, and this patcher will crash.
*/
class	ACharDisplayer : public ATextDisplayer
{
private:
  int 	getMaxH(LPCWSTR str);

public:
  virtual ~ACharDisplayer(){}

  virtual void	display(const DisplayedChar& c) = 0;
  void		display(LPCWSTR str, int x = 0, int y = 0, int w = -1, int h = -1);
  void		display(const std::list<DisplayedChar>& list);
};

#endif /* !CHARDISPLAYER_HPP_ */
