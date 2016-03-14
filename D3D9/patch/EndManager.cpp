#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define	ENDMANAGER_DEFINED
#include	<Windows.h>
#include	<Shellapi.h>
#include	<Shlwapi.h>
#include	"EndManager.hpp"
#include	"Archive.hpp"
#include	"APatch.hpp"
#include	"Reader.hpp"

EndManager*	endManager;
static HHOOK	hHook;

/*
** Important note : this function sets the messages monitoring on the current thread, so it must be called
** from the thread that created the main window.
*/
/*
** TODO: destroy the EndManager (more details below)
** In the constructor, we set a windows hook. If the DLL is unloaded from memory,
** the function pointed by the hook will be unloaded, and calling it will cause a segfault.
** So we should:
** - move the call to UnhookWindowHookEx to the EndManager destructor;
** - and call the EndManager destructor when the patch is unloaded from memory.
*/
EndManager::EndManager()
{
  if (IsDebuggerPresent())
    {
      OutputDebugStringA("Debugger present. Throwing int3 to allow the debugger to put breakpoints.");
      asm("int3");
    }
  GetModuleFileNameW(NULL, this->archiveName, MAX_PATH);
  wcscpy(wcsrchr(this->archiveName, L'\\') + 1, L"archive.tar");
#ifndef DISABLE_ARCHIVE
  this->archive.open(this->archiveName);
#endif /* !DISABLE_ARCHIVE */

  endManager = this;
  hHook = SetWindowsHookExW(WH_CALLWNDPROC, EndManager::windowHook, NULL, GetCurrentThreadId());

  InitializeCriticalSection(&this->cs);
}

EndManager::~EndManager()
{
  DeleteCriticalSection(&this->cs);
}



/*
** Add a file to the list of files saved when the game exits.
*/
void	EndManager::addFile(LPCWSTR filename)
{
  EnterCriticalSection(&this->cs);
  this->files.push_back(filename);
  LeaveCriticalSection(&this->cs);
}

/*
** Tells the end manager to delete the given object.
** The 2nd parameter is the function which will be used to delete the object. You must define this function yourself.
*/
void	EndManager::addObj(void* obj, EndManager::FuncPtr free_func)
{
  this->objs.push_back(std::pair<void*, EndManager::FuncPtr>(obj, free_func));
}

/*
** Tells the end manager to delete the given object.
** The function uses the vtable of EndManager::Object to find the destructor, so the given object must inherit from EndManager::Object.
*/
void	EndManager::addObj(EndManager::Object* obj)
{
  this->addObj(obj, reinterpret_cast<EndManager::FuncPtr>(EndManager::deleteFunc));
}

void	EndManager::deleteFunc(Object* obj)
{
  delete obj;
}

/*
** Checks if the given file is in the archive.
*/
bool	EndManager::isFileInArchive(LPCWSTR filename) const
{
#ifndef DISABLE_ARCHIVE
  return this->archive.hasFile(filename);
#else
  return PathFileExistsW(filename) == TRUE;
#endif /* !DISABLE_ARCHIVE */
}



/*
** Stores all the registered files in an archive.
*/
void	EndManager::storeFiles()
{
#ifdef DISABLE_ARCHIVE
  return ;
#endif /* DISABLE_ARCHIVE */

  for (const std::wstring& file : this->files)
    this->archive.addFile(file, true);
  this->archive.write();
  this->files.empty();
  APatch::get().MessageBoxF(L"Patch note", Reader::get().getNewFilesMessage(), this->archiveName);
}


/*
** Called before each message for the monitored thread.
** When the game finishes, it frees the patch ressources, and stores what have to be stored.
*/
LRESULT CALLBACK	EndManager::windowHook(int nCode, WPARAM wParam, LPARAM lParam)
{
  CWPSTRUCT*		msgParams = (CWPSTRUCT*)lParam;
  static bool		is_released = false;

  if (msgParams->message == WM_DESTROY && is_released == false && APatch::get().isGameExiting(msgParams) == true)
    {
      is_released = true;

      // Note: the objects list must be processed before storing the files, because a destructor
      // may create files (for example, the one of BackgroundTasksManager).
      for (std::pair<void*, EndManager::FuncPtr>& it : endManager->objs)
	it.second(it.first);
      endManager->objs.clear();
      if (!endManager->files.empty())
	endManager->storeFiles();

      delete endManager;
      UnhookWindowsHookEx(hHook);
    }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}
