#include	"BohoConfig/APatch.hpp"
#include	"ATexturesManager.hpp"
#include	"ParseText.hpp"
#include	"Reader.hpp"
#include	"Detour.hpp"
#include	"chcp.hpp"
#include	"Log.hpp"

static Detour<BOOL, HWND, LPCSTR>*			SetWindowTextADetour;
static Detour<BOOL, HWND, UINT, WPARAM, LPARAM>*	SendMessageADetour;

BohoConfig::APatch::APatch()
{
  chcp(932);
  SetWindowTextADetour = new Detour<BOOL, HWND, LPCSTR>();
  SetWindowTextADetour->init("User32.dll", "SetWindowTextA", BohoConfig::APatch::SetWindowTextA);
  SendMessageADetour = new Detour<BOOL, HWND, UINT, WPARAM, LPARAM>();
  SendMessageADetour->init("User32.dll", "SendMessageA", BohoConfig::APatch::SendMessageA);
}

LPCWSTR	BohoConfig::APatch::getMatch(LPCWSTR str)
{
  for (int i = 0; ParseText::strings[i]; i += 2)
    if (ParseText::strings[i + 1] && wcscmp(str, ParseText::strings[i]) == 0)
      return ParseText::strings[i + 1];
  return NULL;
}

BOOL CALLBACK	BohoConfig::APatch::enumProc(HWND hwnd, LPARAM)
{
  DWORD		classNameLen = 10; // The longer string we want to use is ComboBox. If it's longer, it will be truncated.
  WCHAR		className[classNameLen];
  DWORD		winNameLen = GetWindowTextLength(hwnd) + 1;
  WCHAR		winName[winNameLen];
  LPCWSTR	translation;

  GetClassNameW(hwnd, className, classNameLen);
  GetWindowTextW(hwnd, winName, winNameLen);

  if (wcscmp(className, L"ComboBox") == 0)
    {
      int	nbItems;
      int	maxItemLen;
      int	itemLen;
      LPWSTR	item;
      int	sel;

      sel = SendMessageW(hwnd, CB_GETCURSEL, 0, 0);
      maxItemLen = -1;
      item = NULL;
      nbItems = SendMessageW(hwnd, CB_GETCOUNT, 0, 0);
      for (int i = 0; i < nbItems; i++)
	{
	  itemLen = SendMessageW(hwnd, CB_GETLBTEXTLEN, i, 0) + 1;
	  if (itemLen > maxItemLen)
	    {
	      item = (LPWSTR)realloc(item, itemLen * sizeof(WCHAR));
	      maxItemLen = itemLen;
	    }
	  SendMessageW(hwnd, CB_GETLBTEXT, i, (LPARAM)item);
	  translation = getMatch(item);
	  if (translation)
	    {
	      SendMessageW(hwnd, CB_INSERTSTRING, i, (LPARAM)translation);
	      SendMessageW(hwnd, CB_DELETESTRING, i + 1, 0);
	    }
	}
      SendMessageW(hwnd, CB_SETCURSEL, sel, 0);
      free(item);
    }
  else
    {
      translation = getMatch(winName);
      if (translation)
	SetWindowTextW(hwnd, translation);
    }
  return TRUE;
}

void	BohoConfig::APatch::adjustSlider(HWND hText, HWND hSlider)
{
  DWORD	textLen = GetWindowTextLength(hText) + 1;
  WCHAR	text[textLen];
  HDC	hdcText;
  RECT	rText;
  RECT	rSlider;
  int	x;
  int	y;
  int	w;
  int	h;

  GetWindowRect(hText, &rText);
  hdcText = GetDC(hText);
  GetWindowTextW(hText, text, textLen);
  DrawTextW(hdcText, text, textLen - 1, &rText, DT_LEFT | DT_CALCRECT);
  ReleaseDC(hText, hdcText);
  ScreenToClient(GetParent(hText), (LPPOINT)&rText);
  ScreenToClient(GetParent(hText), ((LPPOINT)&rText) + 1);

  GetWindowRect(hSlider, &rSlider);
  ScreenToClient(GetParent(hSlider), (LPPOINT)&rSlider);
  ScreenToClient(GetParent(hSlider), ((LPPOINT)&rSlider) + 1);

  x = rText.right;
  y = rSlider.top;
  w = rSlider.right - x;
  h = rSlider.bottom - rSlider.top;
  SetWindowPos(hText, NULL, rText.left, rText.top, rText.right - rText.left, rText.bottom - rText.top, SWP_NOZORDER);
  SetWindowPos(hSlider, NULL, x, y, w, h, SWP_NOZORDER);
}

void	BohoConfig::APatch::translateWindows(HWND hwnd)
{
  if (Reader::get().initCalled() == false)
    this->init();

  EnumChildWindows(hwnd, BohoConfig::APatch::enumProc, 0);
  SetWindowTextW(hwnd, this->getWindowName());
  adjustSlider(GetDlgItem(hwnd, 1038), GetDlgItem(hwnd, 1005));
}

BOOL WINAPI	BohoConfig::APatch::SetWindowTextA(HWND hwnd, LPCSTR lpString)
{
  static bool	called = false;

  if (strstr(lpString, "Scarlet Family") != 0 || strstr(lpString, "ódÅXñ≤") != 0) // We use strstr with the english part because the japanese part may be broken, and a strstr is faster than an encoding conversion.
    {
      if (called == false)
	dynamic_cast<BohoConfig::APatch&>(APatch::get()).translateWindows(hwnd);
      called = true;
      return TRUE;
    }
  if (GetDlgCtrlID(hwnd) == 1014)
    return SendMessageA(hwnd, WM_SETTEXT, 0, (LPARAM)lpString); // Call to MY SendMessageA, which has a shift-jis to unicode conversion.
  return SetWindowTextADetour->call_orig(hwnd, lpString);
}



/*
** For some things, changing the value returned by chcp isn't enough.
** So we manually do a shift-jis to unicode conversion.
*/
BOOL WINAPI	BohoConfig::APatch::SendMessageA(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if (Msg == CB_ADDSTRING || Msg == WM_SETTEXT)
    {
      LPCSTR	aStr = (LPCSTR)lParam;
      WCHAR	wStr[strlen(aStr) + 1];

      if (MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, aStr, -1, wStr, strlen(aStr) + 1) != 0)
	return SendMessageW(hwnd, Msg, wParam, (LPARAM)wStr);
    }
  return SendMessageADetour->call_orig(hwnd, Msg, wParam, lParam);
}




bool	BohoConfig::APatch::isGameExiting(CWPSTRUCT*) const
{
  return false;
}
