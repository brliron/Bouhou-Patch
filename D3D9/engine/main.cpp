#include	<Windows.h>
// #include	<stdio.h>
#include	"MyD3D9.hpp"
#include	"APatch.hpp"

extern "C" IDirect3D9* WINAPI	MyDirect3DCreate9(UINT SDKVersion)
{
  HINSTANCE			hDll;
  BOOL WINAPI			(*fnIsWow64Process)(HANDLE, PBOOL);
  BOOL				isWow64;
  MyD3D9			*d3d = new MyD3D9();
  IDirect3D9			*d3d_orig;

  // AllocConsole();
  // freopen("CONOUT$", "w", stdout);
  // freopen("dump.txt", "w, ccs=UNICODE", stdout);
  // freopen("dump.txt", "w", stdout);

  fnIsWow64Process = reinterpret_cast<BOOL WINAPI(*)(HANDLE, PBOOL)>(GetProcAddress(GetModuleHandleW(L"Kernel32"), "IsWow64Process"));
  if (fnIsWow64Process && fnIsWow64Process(GetCurrentProcess(), &isWow64) && isWow64 == TRUE)
    hDll = LoadLibraryW(L"C:\\Windows\\SysWOW64\\D3D9.dll");
  else
    hDll = LoadLibraryW(L"C:\\Windows\\System32\\D3D9.dll");
  if (hDll == NULL)
    {
      MessageBoxW(NULL, L"Could not load the original DirectX library (D3D9.dll).\n"
			"Do you have DirectX on your computer ?", NULL, 0);
      return NULL;
    }
  d3d_orig = reinterpret_cast<IDirect3D9*(*)(UINT)>(GetProcAddress(hDll, "Direct3DCreate9"))(SDKVersion);
  if (d3d_orig == NULL)
    return NULL;
  d3d->orig = d3d_orig;
  return d3d;
}


BOOL WINAPI	DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    APatch::get();
  return TRUE;
}
