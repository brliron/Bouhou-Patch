#define _WIN32_WINNT _WIN32_WINNT_VISTA
#include	<Windows.h>

int	main()
{
  CreateSymbolicLink("translations", "..\\translations", SYMBOLIC_LINK_FLAG_DIRECTORY);
  return 0;
}
