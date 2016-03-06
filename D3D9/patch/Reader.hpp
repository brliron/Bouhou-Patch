#ifndef READER_HPP_
# define READER_HPP_

# include	<string>
# include	<vector>
# include	<map>
# include	<Windows.h>
# include	"APatch.hpp"

class	Reader
{
private:
  static Reader*	instance;
  bool			_initCalled;
  WCHAR			base_path[MAX_PATH];
  WCHAR			ret[4096];
  WCHAR			ini_path[MAX_PATH];
  // The key of this map is the message name.
  // The value of this map is a vector. Its 1st value is the message itself,
  // and its other values are the allowed format parameters (use L"%%" to ignore).
  std::map< std::wstring, std::vector<std::wstring> >	messages;

  // Initialize base_path and ini_path
  bool			getPaths(LPCWSTR game);
  // Reads the messages from messages.txt.
  void			readMessages();
  // Check if a printf-like format string have the given format flags.
  bool			checkPercents(LPCWSTR value, const std::vector<std::wstring>& v) const;

public:
  Reader();
  bool			init(LPCWSTR game);
  // Return true if init has already been called, false otherwise.
  bool			initCalled() const;
  // Read the translation files.
  void			readFiles();
  // Read the chars.csv file.
  void			readCharsList();


  // Take a path relative to the translations directory, and return a path relative to the game directory.
  LPCWSTR		getFilePath(LPCWSTR file);
  // Get a value from the INI configuration file.
  LPCWSTR		iniGetString(LPCWSTR section, LPCWSTR key, LPCWSTR def = NULL);
  bool			iniGetBool(LPCWSTR section, LPCWSTR key, bool def = false);

  // Get a message from the messages database.
  const std::wstring&	getMessage(const std::wstring& key) const;
  // Open a MessageBox with a message from the messages database.
  void			dispMessage(const std::wstring& key) const;
  // Open a MessageBox with a formatted message from the messages database.
  template<typename... T>
  void			dispMessageF(const std::wstring& key, T... params) const
  {
    const std::vector<std::wstring>&	v = this->messages.at(key);
    APatch::get().MessageBoxF(!v[0].empty() ? v[0].c_str() : NULL, v[1].c_str(), params...);
  }

  // Reads the file new_files_message.txt, and returns its content.
  LPCWSTR		getNewFilesMessage();

  // Return the global Reader instance, allocating it if needed.
  static Reader&	get();
};

#endif /* !READER_HPP_ */
