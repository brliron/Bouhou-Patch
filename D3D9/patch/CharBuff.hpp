#ifndef CHARBUFF_HPP_
# define CHARBUFF_HPP_

# include	<string>
# include	"chars.hpp"
# include	"EndManager.hpp"

// TODO: replace these defines with static const variables in ACharBuff.
# define	CHARBUFF__NO_FLAGS		0x00
# define	CHARBUFF__DISABLE_BMP_SAVE	0x01	// Don't save unknown characters as bitmaps
# define	CHARBUFF__DISABLE_CHARS_LIST	0x02	// Don't dump characters details into chars_dump.txt
# define	CHARBUFF__DISABLE_LOGS		(CHARBUFF__DISABLE_BMP_SAVE & CHARBUFF__DISABLE_CHARS_LIST)
# define	CHARBUFF__DISABLE_REPLACE_STR	0x04	// Disables the replacing of japanese strings by their translation


/*
** Store all the characters displayed by the game with the add_char function, and displays the translated version with dump_str.
** Each patch must implement the put_char functions to display a character on the screen.
*/
class	ACharBuff : public EndManager::Object
{
private:
  int		flags;

  void	put_str(const wchar_t* str, int x, int y);
  void	dump_line(const std::wstring& str, std::list<DisplayedChar*>& cur_buff);

protected:
  std::list<DisplayedChar*>	buff;

public:
  ACharBuff();
  ACharBuff(std::list<DisplayedChar*>& buff, int flags = CHARBUFF__NO_FLAGS);
  virtual ~ACharBuff();

  void		add_char(Char* c, float x1, float x2, float y1, float y2);
  virtual void	dump_str();
  bool		empty() const;
};

#endif /* !CHARBUFF_HPP_ */
