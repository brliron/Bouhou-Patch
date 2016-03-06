#ifndef ENDMANAGER_HPP_
# define ENDMANAGER_HPP_

# include	<Windows.h>
# include	<list>
# include	<string>
# include	<utility>
# include	<stdio.h>
# include	"Archive.hpp"

/*
** This class detects the end of the game, frees everything the patch allocated, closes opened files,
** and does everything needed at the end of the program.
** It also creates an archive file with every files created by the patch, and asks the user to send me this archive.
*/
class	EndManager
{
public:
  typedef void	(*FuncPtr)(void*);

  class	Object
  {
  public:
    virtual ~Object(){}
  };
private:
  static void		deleteFunc(Object* self);

  CRITICAL_SECTION	cs;
  WCHAR			archiveName[MAX_PATH];
  Archive		archive;

public:
  EndManager();
  ~EndManager();

  std::list<std::wstring>				files;
  std::list< std::pair<void*, EndManager::FuncPtr> >	objs;

  void	addFile(LPCWSTR filename);
  bool	isFileInArchive(LPCWSTR filename) const;
  void	storeFiles();
  void	addObj(void* obj, EndManager::FuncPtr free_func);
  void	addObj(EndManager::Object* obj);

  static LRESULT CALLBACK	windowHook(int nCode, WPARAM wParam, LPARAM lParam);
};

# ifndef ENDMANAGER_DEFINED
extern EndManager*	endManager;
# endif /* !ENDMANAGER_DEFINED */

#endif /* !ENDMANAGER_HPP_ */
