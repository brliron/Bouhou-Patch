#ifndef BACKGROUND_TASKS_MANAGER_HPP_
# define BACKGROUND_TASKS_MANAGER_HPP_

# include	<Windows.h>
# include	"FuncsCallStack.hpp"
# include	"EndManager.hpp"

/*
** This class lets you execute an heavy task in background. You give it an heavy function
** with its arguments, and the BackgroundTasksManager will execute it now or later, in another thread.
** Usage : BackgroundTasksManager::get()->add(heavyFunction, arg1, arg2);
**
** Some important notes :
** - You must allocate an instance of the BackgroundTasksManager before using it
**   (doing "new BackgroundTasksManager();" (without the quotes, of course) is enough)
** - The task may be delayed, but it also may be executed immediately. If you need to
**   free the ressources of an argument, you should do it in the task.
** - A single task should last at most 5 seconds. If it is longer, it may be killed
**   at program termination (the user wants us to stop, we won't make him wait too much)
** - The types are inferred during the template resolution, not during the function call.
**   That means the compiler won't choose to use references, even if the function prototype
**   expects references. If you want to avoid calling the copy constructor of a parameter
**   multiple times, use a pointer or std::ref, or specify the template type manually
**   during the call to BackgroundTasksManager::add.
*/
class	BackgroundTasksManager : public EndManager::Object
{
private:
  static BackgroundTasksManager*	instance;
  FuncsCallStack			funcsCallStack;
  CRITICAL_SECTION			cs;
  HANDLE				event;
  HANDLE				childThread;
  bool					isInit;

  static DWORD CALLBACK			newThreadFunc(LPVOID param);

  void	init();
  DWORD	loop();

public:
  BackgroundTasksManager();
  ~BackgroundTasksManager();

  static BackgroundTasksManager&	get();

  template<typename FuncType, typename... Args>
  void	add(FuncType func, Args... args)
  {
    if (this->isInit == false)
      this->init();
    EnterCriticalSection(&this->cs);
    this->funcsCallStack.emplace(func, args...);
    LeaveCriticalSection(&this->cs);
    SetEvent(this->event);
  }
};

#endif /* !BACKGROUND_TASKS_MANAGER_HPP_ */
