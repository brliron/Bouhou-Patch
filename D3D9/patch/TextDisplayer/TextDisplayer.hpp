#ifndef TEXTDISPLAYER_HPP_
# define TEXTDISPLAYER_HPP_

# include	"chars.hpp"

/*
** Generic interface for displaying text.
** When you have to display a character or a string in the game, use one of the following functions.
** All implementations will have to implement all these functions, and to provide some conversions
** to match the function they were designed for. For example, with a CharDisplayer (built for
** displaying one character each time), the implementation will implement functions with a string
** parameter by calling display one time for each character.
*/
class	ATextDisplayer
{
public:
  virtual ~ATextDisplayer(){}

  void		display(const Char& c, int x, int y);
  void		display(const Char& c, int x1, int x2, int y1, int y2);
  virtual void	display(const DisplayedChar& c) = 0;

  // If w != -1, the text will automatically go to a new line if the newt word would go after x + w.
  // If h != -1 and w != -1, when a new line causes the text to go beyond y + h, a warning is printed to the logs.
  virtual void	display(LPCWSTR str, int x = 0, int y = 0, int w = -1, int h = -1) = 0;

  virtual void	display(const std::list<DisplayedChar>& list) = 0; // Comment to add to StringDisplayer.hpp // With this function, we either have to ignore the content of DisplayedChar, or to draw each character separately. The 1st solution will probably give a better result.
};

#endif /* !TEXTDISPLAYER_HPP_ */
