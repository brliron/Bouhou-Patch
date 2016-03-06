#ifndef PARSE_TEXT_HPP_
# define PARSE_TEXT_HPP_

# include	<list>
# include	<vector>
# include	"chars.hpp"

/*
** Takes a list of DisplayChar, and translate them and / or split them by lines.
** 2 algorithms are available :
** - SPLIT_FIRST : Split the text in lines, then try to translate each line.
**   Best when the texts still aren't known, because this will split the text into lines.
** - TRANSLATE_FIRST : Translate the beginning of the text, then display the untranslated part (if there is one) as is.
**   Best when most of the texts are known, because the texts are taken by blocks instead of individual lines.
*/

class	ParseText
{
public:
  static const int			SPLIT_FIRST = 0;
  static const int			TRANSLATE_FIRST = 1;

private:
  static int				lastType;

  std::list<DisplayedChar*>&		buff;
  int					type;
  const int				flags;
  std::list<DisplayedChar*>::iterator	iterator;

  void					split_lines();
  void					calc_y_shifts_line(std::list<DisplayedChar*>::iterator& it_begin);
  void					calc_all_y_shifts();
  void					replace_lines();
  void					replace_line(std::list<DisplayedChar*>::iterator& begin);
  int					find_line(LPCWSTR str, int& ref_count);
  void					replace_text(const std::list<DisplayedChar*>::iterator& begin);
  void					replace(std::list<DisplayedChar*>::iterator& begin, int length, LPCWSTR new_str, int dest_max_length = INT_MAX);

  static void				skip_line(std::list<DisplayedChar*>::iterator& it, LPCWSTR text, int dest_max_length = INT_MAX);

public:
  ParseText(std::list<DisplayedChar*>& buff, int flags);
  ~ParseText();
  void			parse();
  const DisplayedChar*	get_next_char();

  static std::vector<LPCWSTR>	strings;
};

void	go_to_newline(LPCWSTR& s);

#endif /* !PARSE_TEXT_HPP_ */
