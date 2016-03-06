#ifndef LOG_HPP
# define LOG_HPP_

# include	<string>
# include	<list>
# include	"chars.hpp"
# include	"EndManager.hpp"

/*
** This class formats the unknown texts, ang log them to a text file.
*/
class		Log : public EndManager::Object
{
public:
  static const int	NO_FLAGS		= 0x00;
  static const int	HUMAN_READABLE_OUTPUT	= 0x01;	// Output the logs in a more human-readable, but less easy to parse format (with field name before each value)
  static const int	CREATE_NEW_FILE		= 0x02;	// Create a new log file each time you launch the game (instead of erasing the old one)
  static const int	DISABLE			= 0x04;	// Disable logging through dialogs.txt
  static const int	PRINT_ALL_CHARS		= 0x08;	// After printing a sentence, print detailled logs for every characters (by default, it prints only for unknown characters)
  static const int	PRINT_TRANSLATION	= 0x10;	// After printing unknown characters, print the translation given by CharBuff
  static const int	HIDE_KNOWN_TEXTS	= 0x20;	// Don't print texts with an english translation

private:
  int		flags;
  FILE*		fd;
  bool		init;
  std::wstring	charbuff_text;
  std::wstring	last_charbuff_text;
  std::wstring	last_put_text;
  std::wstring	put_text_pending;
  bool		text_written;

  void		open_file();

public:
  Log();
  ~Log();
  void	begin(const std::list<DisplayedChar*>& buff);
  void	line(const std::list<DisplayedChar*>::iterator& begin, const std::list<DisplayedChar*>::iterator& end, LPCWSTR en_text);
  void	line_with_nl(std::list<DisplayedChar*>::iterator it, const std::list<DisplayedChar*>::iterator& end, LPCWSTR en_text);
  void	end();

  void	put_text(const wchar_t* str);
  bool	open_file(LPCWSTR filename);
};

extern "C" void	log__put_text(const wchar_t* str);

#ifndef LOG_DEFINED
extern Log	*my_log;
#endif /* !LOG_DEFINED */

#endif /* !LOG_HPP_ */
