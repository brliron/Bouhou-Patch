#include	"AD3DPatch.hpp"

D3D9::ATexturesManager*	AD3DPatch::getD3D9TexturesManager()
{
  if (this->texturesManager == nullptr)
    this->texturesManager = this->newTexturesManager();
  return dynamic_cast<D3D9::ATexturesManager*>(this->texturesManager);
}

AD3DPatch::AD3DPatch()
  : D3DDevice(nullptr)
{}

AD3DPatch::~AD3DPatch()
{}

D3DDevice9*	AD3DPatch::getD3DDevice()
{
  if (this->D3DDevice == nullptr)
    this->D3DDevice = this->newD3DDevice();
  return this->D3DDevice;
}

AD3DPatch*	AD3DPatch::get()
{
  return dynamic_cast<AD3DPatch*>(&APatch::get());
}

/*
** If the game is in fullscreen mode, we must minimize it before opening the popup,
** else it will stay in fullscreen mode and freeze.
*/
void	AD3DPatch::MessageBox(LPCWSTR text, LPCWSTR title) const
{
  if (this->isFullscreen)
    {
      ShowWindow(this->getHwnd(), SW_MINIMIZE);
      MessageBoxW(NULL, text, title, 0);
      ShowWindow(this->getHwnd(), SW_RESTORE);
    }
  else
    MessageBoxW(this->getHwnd(), text, title, 0);
}

void	AD3DPatch::initD3D(HWND hFocusWindow, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  HWND	hwnd;

  hwnd = pPresentationParameters->hDeviceWindow;
  if (hwnd == NULL)
    hwnd = hFocusWindow;
  this->setHwnd(hwnd);
  this->isFullscreen = (pPresentationParameters->Windowed == FALSE ? true : false);
}


bool	AD3DPatch::hasInstance()
{
  return APatch::instance != nullptr && dynamic_cast<AD3DPatch*>(APatch::instance) != nullptr;
}
