HRESULT	IDirect3DSurface9::QueryInterface(REFIID riid, void** ppvObj)
{
  printf("QueryInterface\n");
  return orig->QueryInterface(riid, ppvObj);
}

ULONG	IDirect3DSurface9::AddRef()
{
  printf("AddRef\n");
  return orig->AddRef();
}

ULONG	IDirect3DSurface9::Release()
{
  printf("Release\n");
  return orig->Release();
}

HRESULT	IDirect3DSurface9::GetDevice(IDirect3DDevice9** ppDevice)
{
  printf("GetDevice\n");
  return orig->GetDevice(ppDevice);
}

HRESULT	IDirect3DSurface9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
  printf("SetPrivateData\n");
  return orig->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT	IDirect3DSurface9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
  printf("GetPrivateData\n");
  return orig->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT	IDirect3DSurface9::FreePrivateData(REFGUID refguid)
{
  printf("FreePrivateData\n");
  return orig->FreePrivateData(refguid);
}

DWORD	IDirect3DSurface9::SetPriority(DWORD PriorityNew)
{
  printf("SetPriority\n");
  return orig->SetPriority(PriorityNew);
}

DWORD	IDirect3DSurface9::GetPriority()
{
  printf("GetPriority\n");
  return orig->GetPriority();
}

void	IDirect3DSurface9::PreLoad()
{
  printf("PreLoad\n");
  return orig->PreLoad();
}

D3DRESOURCETYPE	IDirect3DSurface9::GetType()
{
  printf("GetType\n");
  return orig->GetType();
}

HRESULT	IDirect3DSurface9::GetContainer(REFIID riid, void** ppContainer)
{
  printf("GetContainer\n");
  return orig->GetContainer(riid, ppContainer);
}

HRESULT	IDirect3DSurface9::GetDesc(D3DSURFACE_DESC* pDesc)
{
  printf("GetDesc\n");
  return orig->GetDesc(pDesc);
}

HRESULT	IDirect3DSurface9::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
  printf("LockRect\n");
  return orig->LockRect(pLockedRect, pRect, Flags);
}

HRESULT	IDirect3DSurface9::UnlockRect()
{
  printf("UnlockRect\n");
  return orig->UnlockRect();
}

HRESULT	IDirect3DSurface9::GetDC(HDC* phdc)
{
  printf("GetDC\n");
  return orig->GetDC(phdc);
}

HRESULT	IDirect3DSurface9::ReleaseDC(HDC hdc)
{
  printf("ReleaseDC\n");
  return orig->ReleaseDC(hdc);
}

