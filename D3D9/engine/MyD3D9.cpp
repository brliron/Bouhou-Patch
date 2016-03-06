#include	"MyD3D9.hpp"
#include	"APatch.hpp"
#include	"AD3DPatch.hpp"
#include	"EndManager.hpp"

HRESULT	MyD3D9::QueryInterface(REFIID riid, void** ppvObj)
{
  return orig->QueryInterface(riid, ppvObj);
}

ULONG	MyD3D9::AddRef()
{
  return orig->AddRef();
}

ULONG	MyD3D9::Release()
{
  return orig->Release();
}

HRESULT	MyD3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
  return orig->RegisterSoftwareDevice(pInitializeFunction);
}

UINT	MyD3D9::GetAdapterCount()
{
  return orig->GetAdapterCount();
}

HRESULT	MyD3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
  return orig->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT	MyD3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
  return orig->GetAdapterModeCount(Adapter, Format);
}

HRESULT	MyD3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
  return orig->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT	MyD3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
  return orig->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT	MyD3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
  return orig->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT	MyD3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
  return orig->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT	MyD3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
  return orig->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT	MyD3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
  return orig->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT	MyD3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
  return orig->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT	MyD3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
  return orig->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR	MyD3D9::GetAdapterMonitor(UINT Adapter)
{
  return orig->GetAdapterMonitor(Adapter);
}



HRESULT			MyD3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
  D3DDevice9		*d3dd;
  IDirect3DDevice9	*d3dd_orig;
  HRESULT		ret;
  AD3DPatch*		patch = AD3DPatch::get();

  if (!patch || patch->init() == false)
    {
      if (patch)
	delete patch;
      return this->orig->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    }

  d3dd = patch->getD3DDevice();
  endManager->addObj(d3dd);
  patch->getTexturesManager().loadTextures();

  ret = this->orig->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &d3dd_orig);
  patch->initD3D(hFocusWindow, pPresentationParameters);

  endManager->addObj(&patch->getCharBuff());

  d3dd->orig = d3dd_orig;
  *ppReturnedDeviceInterface = d3dd;
  return ret;
}
