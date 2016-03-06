/*
** This program allow you to change the properties of all the characters displayed by the patch.
** To use it, uncomment the lines 125 to 129 in game/char_buff.cpp, recompile the patch, run a game using it, then run this program.
*/

#include	<windows.h>
#include	"chars.hpp"

DisplayedChar*	shmem = NULL;


#define MKEDIT(s, v)		mkedit(hInstance, hwnd, s, shmem->v, TRUE)
#define MKEDITF(s, v)		mkedit(hInstance, hwnd, s, shmem->v, FALSE)
#define HANDLE_EDIT(s, v)	handle_edit(hwnd, LOWORD(wParam), s, (long*)&shmem->v)
#define HANDLE_EDITF(s, v)	handle_edit_float(hwnd, LOWORD(wParam), s, (float*)&shmem->v)

void	handle_edit(HWND parent, int id, LPCWSTR target, long* value)
{
  WCHAR	name[255];

  GetDlgItemTextW(parent, id + 30, name, 255);
  if (wcscmp(name, target) != 0)
    return ;
  *value = GetDlgItemInt(parent, id, NULL, FALSE);
  shmem->dst_y1 = 369 + shmem->sy;
  shmem->dst_x2 = shmem->dst_x1 + shmem->sw;
  shmem->dst_y2 = shmem->dst_y1 + shmem->sh;
  shmem->tx2 = shmem->tx1 + ((float)shmem->sw) / 2048;
}

void	handle_edit_float(HWND parent, int id, LPCWSTR target, float* value)
{
  WCHAR	name[255];

  GetDlgItemTextW(parent, id + 30, name, 255);
  if (wcscmp(name, target) != 0)
    return ;
  GetDlgItemTextW(parent, id, name, 255);
  *value = _wtof(name);
  shmem->tx2 = shmem->tx1 + ((float)shmem->sw) / 2048;
}

LRESULT CALLBACK	main_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
    {
      case WM_COMMAND :
	if (HIWORD(wParam) != EN_CHANGE)
	  break ;
	HANDLE_EDITF(L"tx1", tx1);
	HANDLE_EDITF(L"ty1", ty1);
	// HANDLE_EDITF(L"tx2", tx2);
	HANDLE_EDITF(L"ty2", ty2);
	HANDLE_EDIT(L"sy", sy);
	HANDLE_EDIT(L"sw", sw);
	HANDLE_EDIT(L"sh", sh);
	// HANDLE_EDIT(L"dst_x1", dst_x1);
	// HANDLE_EDIT(L"dst_y1", dst_y1);
	// HANDLE_EDIT(L"dst_x2", dst_x2);
	// HANDLE_EDIT(L"dst_y2", dst_y2);
	return (0);

      case WM_DESTROY :
	PostQuitMessage(0);
	return (0);
    }
  return (DefWindowProc(hwnd, msg, wParam, lParam));
}

void	mkedit(HINSTANCE hInstance, HWND parent, LPCWSTR str, int value, BOOL integers)
{
  WCHAR	avalue[255];
  static int n = 0;

  wsprintfW(avalue, L"%d", value);
  CreateWindowW(L"STATIC", str, WS_VISIBLE | WS_CHILD,
	       10 + 190 * (n / 22), 10 + n % 22 * 25, 120, 18,
	       parent, (HMENU)(n + 31), hInstance, NULL);
  CreateWindowW(L"EDIT", avalue, WS_VISIBLE | WS_CHILD | (integers ? ES_NUMBER : 0),
	       130 + 190 * (n / 22), 10 + n % 22 * 25, 50, 18,
	       parent, (HMENU)(n + 1), hInstance, NULL);
  n++;
}

int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow)
{
  HANDLE	hShm;
  MSG		msg;

  (void)hPrevInstance;
  (void)lpCmdLine;

  hShm = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(DisplayedChar), L"patch_char");
  shmem = (DisplayedChar*)MapViewOfFile(hShm, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DisplayedChar));
  shmem->c = L'A';
  shmem->dst_x1 = 275;
  shmem->dst_y1 = 369;

  WNDCLASSW	wc;

  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = main_proc;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpszClassName = L"Main";
  RegisterClassW(&wc);

  HWND hwnd = CreateWindowW(L"Main", L"Patch character replacing", WS_OVERLAPPEDWINDOW,
	       CW_USEDEFAULT, SW_SHOW, 400, 600,
	       NULL, NULL, hInstance, NULL);
  MKEDITF(L"tx1", tx1);
  MKEDITF(L"ty1", ty1);
  // MKEDITF(L"tx2", tx2);
  MKEDITF(L"ty2", ty2);
  MKEDIT(L"sy", sy);
  MKEDIT(L"sw", sw);
  MKEDIT(L"sh", sh);
  // MKEDIT(L"dst_x1", dst_x1);
  // MKEDIT(L"dst_y1", dst_y1);
  // MKEDIT(L"dst_x2", dst_x2);
  // MKEDIT(L"dst_y2", dst_y2);
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  return (int)(msg.wParam);
}
