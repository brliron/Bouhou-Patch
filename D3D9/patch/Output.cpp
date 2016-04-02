#include	"Output.hpp"
#include	"Reader.hpp"

typedef struct _CONSOLE_FONT_INFOEX {
  ULONG cbSize;
  DWORD nFont;
  COORD dwFontSize;
  UINT  FontFamily;
  UINT  FontWeight;
  WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

Output*	Output::instance;



Output::Output()
  : console(false), fd(NULL), bakFd(NULL)
{
  if (Output::instance)
    delete Output::instance;
  Output::instance = this;

  if (Reader::get().iniGetBool(L"Output", L"CONSOLE"))
    this->openConsole();
  if (Reader::get().iniGetBool(L"Output", L"FILE"))
    this->openFile();
}

Output::~Output()
{
  if (fd)
    fclose(fd);
}

void	Output::free()
{
  if (Output::instance)
    {
      delete Output::instance;
      Output::instance = NULL;
    }
}



void			Output::_openConsole()
{
  if (this->console == true)
    return ;

  AllocConsole();
  // freopen("CONOUT$", "w", stdout); // Do I want to keep that?
  SetStdHandle(STD_OUTPUT_HANDLE, CreateFileW(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)); // TODO: error checking.
  SetConsoleOutputCP(CP_UTF8);
  /*
  ** This block is currently disabled.
  ** See the comment in Output.hpp above setFontXP for more details.

  if (GetProcAddress(GetModuleHandle("Kernel32"), "SetCurrentConsoleFontEx") != NULL)
    this->setFontVista();
  else
    this->setFontXP();

  */
  this->console = true;
}

/*
** This block is currently disabled.
** See the comment in Output.hpp above setFontXP for more details.

// Tries to set the console font to Lucida Console on Windows XP
// Current status : works on my machine, but sets a tiny font size.
void	Output::setFontXP() const
{
  BOOL WINAPI	(*SetConsoleFontPtr)(HANDLE hOutput, DWORD fontIndex);

  ::printf("XP\n");
  SetConsoleFontPtr = (BOOL WINAPI (*)(HANDLE, DWORD))GetProcAddress(GetModuleHandle("Kernel32"), "SetConsoleFont");

  HKEY key;
  RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Console\\TrueTypeFont", 0, KEY_READ, &key);
  WCHAR	name[4096]; // Note : 4096 is an arbitrary value.
  DWORD	name_len = 4096;
  WCHAR	data[4096];
  DWORD data_len = 4096;
  int i = 0;
  while (RegEnumValueW(key, i, name, &name_len, NULL, NULL, (BYTE*)data, &data_len) != ERROR_NO_MORE_ITEMS)
    {
      if (wcscmp(data, L"Lucida Console") == 0)
	{
	  int call_ret = SetConsoleFontPtr(GetStdHandle(STD_OUTPUT_HANDLE), i);
	  ::printf("Result of calling SetConsoleFont with %d : %d\n", i, call_ret);
	  return ;
	}
      i++;
      name_len = 4096;
      data_len = 4096;
    }

  this->_write(L"Warning : could not find the Lucida Console font.\n"
	       "The unicode characters may not be displayed correctly.");
}

// Set the console font to Lucida Console on Windows Vista and above.
void	Output::setFontVista() const
{
  CONSOLE_FONT_INFOEX	font;
  BOOL WINAPI	(*GetCurrentConsoleFontExPtr)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
  BOOL WINAPI	(*SetCurrentConsoleFontExPtr)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);

  ::printf("Vista\n");
  GetCurrentConsoleFontExPtr = (BOOL WINAPI (*)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX))GetProcAddress(GetModuleHandle("Kernel32"), "GetCurrentConsoleFontEx");
  SetCurrentConsoleFontExPtr = (BOOL WINAPI (*)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX))GetProcAddress(GetModuleHandle("Kernel32"), "SetCurrentConsoleFontEx");

  font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontExPtr(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);
  if (wcscmp(font.FaceName, L"Lucida Console") == 0)
    return ;
  // nFont, FontFamily and dwFontSize.X are ignored ? Well, let's try to set them to 0.
  font.nFont = 0;
  font.FontFamily = 0;
  font.dwFontSize.X = 0;

  font.dwFontSize.Y = 14; // font size
  font.FontWeight = FW_NORMAL;
  wcscpy(font.FaceName, L"Lucida Console");
  // If we want to display Japanese and Latin characters, MS Gothic may be a good idea.
  // But it won't be available on a computer without the Japanese language pack, so we should
  // be able to fall back to Lucida Console if it didn't work.
  // wcscpy(font.FaceName, L"ＭＳ ゴシック");
  int call_ret = SetCurrentConsoleFontExPtr(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);
  ::printf("Result of calling SetCurrentConsoleFontEx with Lucida Console : %d\n", call_ret);
}

*/

bool	Output::_openFile()
{
  if (this->fd != NULL)
    return true;
  this->fd = fopen("translations\\log.txt", "w, ccs=UNICODE");
  if (this->fd == NULL)
    {
      this->write(L"Error opening log file translations\\log.txt\n");
      return false;
    }
  return true;
}

void	Output::setFilePointer(FILE* fd)
{
  if (!Output::instance->bakFd)
    Output::instance->bakFd = Output::instance->fd;
  Output::instance->fd = fd;
}

void	Output::resetFilePointer()
{
  Output::instance->fd = Output::instance->bakFd;
}




void	Output::_write(LPCWSTR str)
{
  this->toCout(str);
  this->toFile(str);
}

void	Output::_toCout(LPCWSTR str) const
{
  if (!this->console)
    return ;

  DWORD dummy;
  WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str, wcslen(str), &dummy, NULL);
}

void	Output::_toFile(LPCWSTR str)
{
  if (!this->fd)
    return ;
  fputws(str, this->fd);
  fflush(this->fd);
}



void	Output::openConsole()
{
  Output::instance->_openConsole();
}

bool	Output::openFile()
{
  return Output::instance->_openFile();
}

void	Output::write(LPCWSTR str)
{
  Output::instance->_write(str);
}

void	Output::toCout(LPCWSTR str)
{
  Output::instance->_toCout(str);
}

void	Output::toFile(LPCWSTR str)
{
  Output::instance->_toFile(str);
}
