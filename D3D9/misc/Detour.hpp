#ifndef DETOUR_HPP_
# define DETOUR_HPP_

# include	<Windows.h>
/*
** Note : for now, the detourned function must use stdcall calling convention, and it must return something.
*/
template <typename T, typename... Args>
class		Detour
{
public:
  typedef T (WINAPI *FuncType)(Args...);

private:
  CRITICAL_SECTION	CriticalSection;
  FuncType		func;
  BYTE			save[10];

  // Custom memcpy to remove dependencies to the libc
  void	memcpy(void* dest_, const void* src_, unsigned long n)
  {
    char*	dest = (char*)dest_;
    const char*	src  = (const char*)src_;

    while (n > 0)
      {
	*dest = *src;
	dest++;
	src++;
	n--;
      }
  }

  // Custom memcmp to remove dependencies to the libc
  int	memcmp(const void* s1_, const void* s2_, unsigned long n)
  {
    const char*	s1 = (const char*)s1_;
    const char*	s2 = (const char*)s2_;

    while (n > 0)
      {
	if (*s1 != *s2)
	  return *s1 - *s2;
	s1++;
	s2++;
	n--;
      }
    return 0;
  }

public:
  Detour()
  {
    InitializeCriticalSection(&this->CriticalSection);
  }

  ~Detour()
  {
    if (this->func == nullptr)
      return ;
    EnterCriticalSection(&this->CriticalSection);

    BYTE*	code;
    // DWORD	save;
    // DWORD	dummy;

    code = (BYTE*)this->func;
    // VirtualProtect(code, 10, PAGE_EXECUTE_READWRITE, &save);
    Detour::memcpy(code, this->save, 10);
    FlushInstructionCache(GetCurrentProcess(), code, 10);
    // VirtualProtect(code, 10, save, &dummy);

    LeaveCriticalSection(&this->CriticalSection);
    DeleteCriticalSection(&this->CriticalSection);
  }


  /*
  ** Important note concerning the 1st init function: the 1st parameter must be a pointer to a function, not a pointer to something in the PLT.
  ** Use GetProcAddress to find the function's address, or use the 2nd prototype.
  */
  void		init(FuncType old_func, FuncType new_func)
  {
    BYTE*	code;
    DWORD	save;
    // DWORD	dummy;

    EnterCriticalSection(&this->CriticalSection);

    code = (BYTE*)old_func;
    this->func = old_func;
    Detour::memcpy(this->save, code, 10);

    VirtualProtect(code, 10, PAGE_EXECUTE_READWRITE, &save);

    // jmp *(code + 6) : absolute indirect jump to the address stored at code + 6.
    code[0] = 0xff;
    code[1] = 0x25;
    *(BYTE**)(code + 2) = code + 6;
    // Address used by the absolute indirect jump above.
    *(void**)(code + 6) = (void*)new_func;

    if (Detour::memcmp(code, this->save, 10) == 0)
      MessageBoxA(NULL, "Warning : Detour::init() seemed to be called twice !", NULL, MB_OK);

    FlushInstructionCache(GetCurrentProcess(), code, 10);
    // VirtualProtect(code, 10, save, &dummy); // It seems like VirtualProtect stops working if it's called too often, so I stop calling it at each call and keep the memory protection like this.

    LeaveCriticalSection(&this->CriticalSection);
  }


  void		init(LPCTSTR module, LPCSTR old_func, FuncType new_func)
  {
    FARPROC	proc;

    proc = GetProcAddress(GetModuleHandle(module), old_func);
    return this->init((FuncType)proc, new_func);
  }


  T		call_orig(Args... params)
  {
    BYTE*	code;
    BYTE	detour[10];
    T		ret;
    // DWORD	save;
    // DWORD	dummy;

    EnterCriticalSection(&this->CriticalSection);

    code = (BYTE*)this->func;
    // VirtualProtect(code, 10, PAGE_EXECUTE_READWRITE, &save);
    Detour::memcpy(detour, code, 10);
    Detour::memcpy(code, this->save, 10);
    FlushInstructionCache(GetCurrentProcess(), code, 10);
    // VirtualProtect(code, 10, save, &dummy);

    ret = this->func(params...);

    // VirtualProtect(code, 10, PAGE_READWRITE, &save);
    Detour::memcpy(code, detour, 10);
    FlushInstructionCache(GetCurrentProcess(), code, 10);
    // VirtualProtect(code, 10, save, &dummy);

    LeaveCriticalSection(&this->CriticalSection);
    return ret;
  }
};

#endif /* !DETOUR_HPP_ */
