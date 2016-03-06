#ifndef APATCH_HPP_
# define APATCH_HPP_

# include	<stdio.h>
# include	"ATexturesManager.hpp"
# include	"chars.hpp"

/*
** The base class for all game-specific code.
** It loads the translations and allocates the correct subclasses for the running game.
** Each game must complete it.
*/
class	APatch
{
private:
  HWND			hwnd;

protected:
  static APatch*	instance;
  ATexturesManager*	texturesManager;
  ACharBuff*		charBuff;

  // TODO: all these pure functions become annoying when I don't intend to use them.
  // I should put them somewhere else, or add default functions.
  virtual ATexturesManager*	newTexturesManager() const = 0;
  virtual ACharBuff*		newCharBuff(int flags = CHARBUFF__NO_FLAGS) const = 0;
  virtual LPCWSTR		getGameName() const = 0;
  virtual LPCWSTR		getWindowName() const = 0;

  // Register the game main HWND.
  void				setHwnd(HWND hwnd);

  // Read the translation files.
  void				readFiles();
  // Read the chars.csv file.
  void				readCharsList();

public:
  APatch();
  virtual ~APatch();

  // Performs various initialisation tasks.
  bool			init();

  // Called by the end manager when a WM_CLOSE or WM_DESTROY message is sent to the thread monitored by the EndManager.
  // Return true if this message means the game is exiting, false otherwise.
  // The default implementation throws an exception. You must implement it if you use the end manager. TODO: make a default implementation.
  virtual bool		isGameExiting(CWPSTRUCT* msgParams) const = 0;
  // Calls MessageBoxW, and ensures the spawned message box won't cause problems (see implementation in Boho for an example).
  // The default implementation redirects to MessageBoxW.
  virtual void		MessageBox(LPCWSTR text, LPCWSTR title = NULL) const;
  // Formatted message box using the printf syntax.
  template<typename... T>
  void			MessageBoxF(LPCWSTR title, LPCWSTR format, T... params)
  {
    WCHAR		buff[4096];
    buff[4095] = L'\0';
    _snwprintf(buff, 4095, format, params...);
    this->MessageBox(buff, title);
  }

  // Return the instances of TexturesManager and CharBuff associated with APatch, allocating them if needed.
  ATexturesManager&	getTexturesManager();
  ACharBuff&		getCharBuff();

  // Get the game main HWND.
  HWND			getHwnd() const;

  // Return the global APatch instance, allocating it if needed.
  static APatch&	get();
  // Return true if the global APatch instance is already allocated, false otherwise.
  static bool		hasInstance();

  // Check if all the patch files are correct.
  void			selfTest();
};

#endif /* !APATCH_HPP_ */
