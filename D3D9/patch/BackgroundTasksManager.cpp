#include	"BackgroundTasksManager.hpp"

BackgroundTasksManager*	BackgroundTasksManager::instance = NULL;


BackgroundTasksManager::BackgroundTasksManager()
  : event(NULL), childThread(NULL), isInit(false)
{
  BackgroundTasksManager::instance = this;
}

BackgroundTasksManager::~BackgroundTasksManager()
{
  BackgroundTasksManager::instance = NULL;

  if (this->isInit == false)
    return ;

  unsigned long	lastStackSize;
  unsigned long	curStackSize;

  curStackSize = this->funcsCallStack.size();
  this->isInit = false;
  // TODO: add a progress bar around here
  do
    {
      lastStackSize = curStackSize;
      SetEvent(this->event);
      WaitForSingleObject(this->childThread, 5000); // We'll wait at most 5 sec per task.
      curStackSize = this->funcsCallStack.size();
    } while (curStackSize != 0 && curStackSize != lastStackSize);
    if (curStackSize != 0) // timeout
      TerminateThread(this->childThread, 1);

  CloseHandle(this->childThread);
  CloseHandle(this->event);
  DeleteCriticalSection(&this->cs);
}

void	BackgroundTasksManager::init()
{
  this->isInit = true;
  InitializeCriticalSection(&this->cs);
  this->event = CreateEvent(NULL, FALSE, FALSE, NULL);
  this->childThread = CreateThread(NULL, 0, BackgroundTasksManager::newThreadFunc, this, 0, NULL);
}

DWORD CALLBACK	BackgroundTasksManager::newThreadFunc(LPVOID param)
{
  return reinterpret_cast<BackgroundTasksManager*>(param)->loop();
}

DWORD	BackgroundTasksManager::loop()
{
  while (isInit)
    {
      WaitForSingleObject(this->event, INFINITE);
      while (!this->funcsCallStack.empty())
	{
	  this->funcsCallStack.call();
	  EnterCriticalSection(&this->cs);
	  this->funcsCallStack.pop();
	  LeaveCriticalSection(&this->cs);
	}
    }
  return 0;
}


BackgroundTasksManager&	BackgroundTasksManager::get()
{
  return *BackgroundTasksManager::instance;
}

