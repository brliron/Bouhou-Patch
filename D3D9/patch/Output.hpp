#ifndef OUTPUT_HPP_
# define OUTPUT_HPP_

# include	<Windows.h>
# include	<stdio.h>

/*
** This class enables logging to the console, or to a file.
** To initialise it, use :
** new Output();
** You can enable / disable the logging through the config file.
** see the config file's readme for more details.
** To free it, use :
** Output::free();
** To log something, use :
** Output::write(L"some text\n");
** or :
** Output::printf(format_string, params...);
** For example :
** Output::printf(L"format string %s %d\n", L"string param", 2);
*/

class	Output
{
private:
  static Output*	instance;

  bool	console;
  FILE*	fd;
  FILE*	bakFd;
  WCHAR	buff[4096];

  /*
  ** These fonctions change the console font on Windows XP for the 1st one,
  ** and for Windows Vista and later for the 2nd one.
  ** Changing the console font allows us to display unicode characters in the console.
  ** But for now, we don't use these fonctions for 2 reasons :
  ** - We don't need to display unicode text. All the messages displayed by this function
  **   are in english for now.
  ** - The results aren't really consistent between computers. Some will use MS Gothic,
  **   most of them will use Lucida Console, and some will use a tiny version of
  **   Lucida Console. And except if we want to display, for example, Cycillic text,
  **   the only font that matters is MS Gothic ; it's the only one able do display the
  **   games' texts.

  void	setFontXP() const;
  void	setFontVista() const;
  */

  template<typename... T>
  LPCWSTR	_format(LPCWSTR format, T... args)
  {
    this->buff[4095] = L'\0';
    _snwprintf(this->buff, 4095, format, args...);
    return this->buff;
  }

  void	_openConsole();
  bool	_openFile();

  void	_write(LPCWSTR str);
  void	_toCout(LPCWSTR str) const;
  void	_toFile(LPCWSTR str);

public:
  Output();
  ~Output();

  // Note : these 2 functions are called in the constructor according to the value in the config file.
  static void	openConsole();
  static bool	openFile();
  // Temporary changes the Output file pointer.
  static void	setFilePointer(FILE* fd);
  // Reset the Output file pointer to its value before calling setFilePointer.
  static void	resetFilePointer();

  // The global output instance needs to be allocated by doing
  // new Output();
  // You don't need to store the result. To destroy the instance
  // allocated by this line, use the following function.
  static void	free();

  // Quite like sprintf, but safer and easier to use.
  // But each call to this function overwrite the string of the precedent call.
  // Intended to be used by Output::printf.
  template<typename... T>
  static LPCWSTR	format(LPCWSTR format, T... args)
  {
    return Output::instance->_format(format, args...);
  }

  // Prints some texts with Output::write using a printf-like syntax.
  template<typename... T>
  static void	printf(LPCWSTR format, T... args)
  {
    Output::write(Output::format(format, args...));
  }

  // Write some text to both the console and the log file (if they are enabled).
  static void	write(LPCWSTR str);
  // Write some text to the console (if logging to the console is enabled).
  static void	toCout(LPCWSTR str);
  // Write some text to the log file (if logging to the log file is enabled).
  static void	toFile(LPCWSTR str);
};

#endif /* !OUTPUT_HPP_ */
