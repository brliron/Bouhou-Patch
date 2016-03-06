#include	<Windows.h>
#include	"chcp.hpp"

void	chcp(DWORD cp)
{
  BYTE*	code;
  BYTE*	byteCp;
  DWORD	save;
  DWORD	dummy;

  code = (BYTE*)GetProcAddress(GetModuleHandle("Kernel32.dll"), "GetACP");
  VirtualProtect(code, 6, PAGE_EXECUTE_READWRITE, &save);
  byteCp = (BYTE*)&cp;

  // mov eax, cp
  code[0] = 0xb8;
  code[1] = byteCp[0];
  code[2] = byteCp[1];
  code[3] = byteCp[2];
  code[4] = byteCp[3];
  // ret
  code[5] = 0xc3;

  FlushInstructionCache(GetCurrentProcess(), code, 6);
  VirtualProtect(code, 6, save, &dummy);
}
