#include	<Windows.h>

HMODULE		load_lib()
{
  HMODULE	hLib;
  BOOL WINAPI	(*fnIsWow64Process)(HANDLE, PBOOL);
  BOOL		isWow64;

  fnIsWow64Process = (BOOL WINAPI(*)(HANDLE, PBOOL))GetProcAddress(GetModuleHandleW(L"Kernel32"), "IsWow64Process");
  if (fnIsWow64Process && fnIsWow64Process(GetCurrentProcess(), &isWow64) && isWow64 == TRUE)
    hLib = LoadLibrary("C:\\Windows\\SysWOW64\\" LIB_NAME ".dll");
  else
    hLib = LoadLibrary("C:\\Windows\\System32\\" LIB_NAME ".dll");
  if (hLib == NULL)
    {
      MessageBoxW(NULL, L"Could not load the original " LIB_NAME ".dll .\n" CUSTOM_ERR, NULL, 0);
      return NULL;
    }
  return hLib;
}

void WINAPI		FindEntry(const char* name, void** address)
{
  static HMODULE	hLib = NULL;

  if (hLib == NULL)
    hLib = load_lib();
  *address = GetProcAddress(hLib, name);
}
