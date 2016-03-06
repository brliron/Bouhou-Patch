#define	_WIN32_WINNT	_WIN32_WINNT_WINXP
#define	_WIN32_IE	_WIN32_IE_IE80
#define	UNICODE
#define	_UNICODE
#include	<Windows.h>
#include	<Commctrl.h>

// These 2 defines are part of the WinAPI, but it seems MinGW doesn't define them.
#define	UIS_SET		1
#define	UISF_HIDEFOCUS	1

#define	MARGIN_LR	pt_to_px(15) // Margin left / right
#define	MARGIN_TD	pt_to_px(16) // Margin tom / bottom
#define	MARGIN_Y	pt_to_px(14) // Margin between the elements
#define	BUTTON_W	pt_to_px(60)
#define	BUTTON_H	pt_to_px(18)
#define	BUTTON_INDENT	pt_to_px(18)

static int	pt_to_px(int pt)
{
  static int	save[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  if (pt < 20 && save[pt] != 0)
    return save[pt];

  HDC		hdc;
  int		height;

  hdc = GetDC(NULL);
  height = MulDiv(pt, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(NULL, hdc);

  save[pt] = height;
  return height;
}

static HFONT	get_font()
{
  static HFONT	font = NULL;

  if (font == NULL)
    font = CreateFontW(-pt_to_px(8), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
  /*
  ** About the font choice, you can read this : http://stackoverflow.com/questions/6057239/which-font-is-the-default-for-mfc-dialog-controls
  ** I want to be XP-compatible, I'm too lazy to do the thing described in the article and to thoughtfully test it,
  ** Windows 7 often uses Tahoma, and my box has a XP-like style anyway, so I'll just use Tahoma.
  */
  return font;
}

static LRESULT CALLBACK	DropdownBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static int*		ret;
  CREATESTRUCT*		param;

  switch (msg)
    {
    case WM_CREATE:
      param = (CREATESTRUCT*)lParam;
      ret = (int*)param->lpCreateParams;
      *ret = -1;
      break ;

    case WM_COMMAND:
      switch (LOWORD(wParam))
	{
	case 101: // DropDown list
	  if (HIWORD(wParam) == CBN_SELCHANGE)
	    *ret = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
	  return 0;
	case IDOK:
	  DestroyWindow(hwnd);
	  return 0;
	case IDCANCEL:
	  *ret = -1;
	  DestroyWindow(hwnd);
	  return 0;
	}
      break ;

    case WM_CLOSE:
      *ret = -1;
      break ;
    }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

static void	enable_visual_styles()
{
  WCHAR		dir[MAX_PATH];
  ULONG_PTR	cookie;
  ACTCTX	ctx;

  GetSystemDirectoryW(dir, MAX_PATH);
  ctx.cbSize = sizeof(ACTCTX);
  ctx.dwFlags = ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_RESOURCE_NAME_VALID;
  ctx.lpSource = L"shell32.dll";
  ctx.wProcessorArchitecture = 0;
  ctx.wLangId = 0;
  ctx.lpAssemblyDirectory = dir;
  ctx.lpResourceName = MAKEINTRESOURCE(124); // TODO: see in a resource editor.
  ctx.lpApplicationName = NULL;
  ctx.hModule = NULL;
  ActivateActCtx(CreateActCtx(&ctx), &cookie);
}

static void	register_class()
{
  WNDCLASSW	wndClass;
  if (GetClassInfoW(GetModuleHandle(NULL), L"DropdownBox", &wndClass))
    return ;

  enable_visual_styles();

  INITCOMMONCONTROLSEX	initCtrls;
  initCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  initCtrls.dwICC = ICC_STANDARD_CLASSES;
  InitCommonControlsEx(&initCtrls);

  wndClass.style = 0;
  wndClass.lpfnWndProc = DropdownBoxProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = GetModuleHandle(NULL);
  wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // TODO: change ?
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = L"DropdownBox";
  RegisterClassW(&wndClass);
}

/*
** pt :
** In: x and y pos.
** Out: w and h.
*/
static HWND	create_static(LPCWSTR text, POINT* pt, HWND parent)
{
  HWND		hwnd;
  HDC		hdc;
  HGDIOBJ	oldFont;
  RECT		rect;

  hwnd = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
		       pt->x, pt->y, 500, 100,
		       parent, (HMENU)100, GetModuleHandle(NULL), NULL);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)get_font(), TRUE);

  rect.left = 0;
  rect.top = 0;
  rect.right = 500;
  rect.bottom = 100;
  hdc = GetDC(hwnd);
  oldFont = SelectObject(hdc, get_font());
  DrawTextW(hdc, text, wcslen(text), &rect, DT_LEFT | DT_CALCRECT);
  SelectObject(hdc, oldFont);
  ReleaseDC(hwnd, hdc);
  pt->x = rect.right - rect.left;
  pt->y = rect.bottom - rect.top;
  SetWindowPos(hwnd, NULL, -1, -1, pt->x, pt->y, SWP_NOMOVE | SWP_NOZORDER);
  return hwnd;
}

static HWND	create_combobox(LPCWSTR* items, POINT* pt, HWND parent)
{
  HWND		hwnd;
  HDC		hdc;
  RECT		rect;
  int		max_width;
  int		width;
  int		elem_height;
  int		height;
  int		i;

  pt->y += MARGIN_TD + MARGIN_Y;
  hwnd = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST,
		       MARGIN_LR, pt->y, 500, 500,
		       parent, (HMENU)101, GetModuleHandle(NULL), NULL);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)get_font(), TRUE);
  hdc = GetDC(hwnd);

  rect.left = 0;
  rect.top = 0;
  rect.right = 500;
  rect.bottom = 100;
  DrawTextW(hdc, L"I", wcslen(L"I"), &rect, DT_LEFT | DT_CALCRECT);
  elem_height = rect.bottom - rect.top;
  pt->y += elem_height + 4;
  elem_height += 10; // We set the maximum size, not the actual size. So we can take some margin.

  max_width = pt->x;
  width = 0;
  height = elem_height;
  for (i = 0; items[i]; i++)
    {
      rect.left = 0;
      rect.top = 0;
      rect.right = max_width + 200;
      rect.bottom = height;
      DrawTextW(hdc, items[i], wcslen(items[i]), &rect, DT_LEFT | DT_CALCRECT);
      if (rect.right - rect.left > width)
	width = rect.right - rect.left;
      height += elem_height;

      SendMessageW(hwnd, CB_ADDSTRING, 0, (LPARAM)items[i]);
    }
  width += pt_to_px(15); // The space before and after the text.
  if (width > max_width)
    width = max_width;

  SetWindowPos(hwnd, NULL, -1, -1, width, height, SWP_NOMOVE | SWP_NOZORDER);
  ReleaseDC(hwnd, hdc);
  return hwnd;
}

static void	init_main_win(HWND hwnd, POINT* pt)
{
  HWND		ok_hwnd;
  HWND		cancel_hwnd;
  int		cancel_x;
  int		ok_x;

  if (pt->x < BUTTON_INDENT + BUTTON_W + pt_to_px(5) + BUTTON_W)
    pt->x = BUTTON_INDENT + BUTTON_W + pt_to_px(5) + BUTTON_W;
  cancel_x = pt->x - BUTTON_W;
  ok_x = cancel_x - pt_to_px(5) - BUTTON_W;
  pt->y += MARGIN_Y * 1.5;
  cancel_hwnd = CreateWindowW(L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER | BS_PUSHBUTTON,
			      MARGIN_LR + cancel_x, pt->y, BUTTON_W, BUTTON_H,
			      hwnd, (HMENU)IDCANCEL, GetModuleHandle(NULL), NULL);
  ok_hwnd = CreateWindowW(L"BUTTON", L"Ok", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER | BS_PUSHBUTTON,
			  MARGIN_LR + ok_x, pt->y, BUTTON_W, BUTTON_H,
			  hwnd, (HMENU)IDOK, GetModuleHandle(NULL), NULL);
  SendMessage(cancel_hwnd, WM_SETFONT, (WPARAM)get_font(), TRUE);
  SendMessage(ok_hwnd, WM_SETFONT, (WPARAM)get_font(), TRUE);
  pt->y += BUTTON_H;

  SendMessage(hwnd, WM_CHANGEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);

  HMONITOR	monitor;
  MONITORINFO	info;
  RECT		rect;
  monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  info.cbSize = sizeof(info);
  GetMonitorInfo(monitor, &info);
  rect.right  = pt->x + 2 * MARGIN_LR;
  rect.bottom = pt->y + MARGIN_TD;
  rect.left   = info.rcWork.left + (info.rcWork.right  - info.rcWork.left) / 2 - rect.right / 2;
  rect.top  = info.rcWork.top  + (info.rcWork.bottom - info.rcWork.top ) / 2 - rect.bottom / 2;
  rect.right  += rect.left;
  rect.bottom += rect.top;
  AdjustWindowRect(&rect, WS_POPUPWINDOW | WS_CAPTION, FALSE);
  SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

  ShowWindow(hwnd, SW_SHOW);
}

int	DropdownBox(LPCWSTR title, LPCWSTR text, LPCWSTR* items)
{
  HWND	main_hwnd;
  MSG	msg;
  POINT	pt;
  int	ret;

  register_class();
  main_hwnd = CreateWindowW(L"DropdownBox", title, WS_POPUPWINDOW | WS_CAPTION,
			    CW_USEDEFAULT, SW_SHOW, 500, 200,
			    NULL, NULL, GetModuleHandle(NULL), &ret);
  pt.x = MARGIN_LR;
  pt.y = MARGIN_TD;
  create_static(text, &pt, main_hwnd);
  create_combobox(items, &pt, main_hwnd);
  init_main_win(main_hwnd, &pt);

  while (IsWindow(main_hwnd) && GetWindowThreadProcessId(main_hwnd, NULL) == GetCurrentThreadId() && GetMessage(&msg, NULL, 0, 0) > 0)
    if (IsDialogMessage(main_hwnd, &msg) == 0)
      {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
      }
  return ret;
}
