#include	"D3DDevice9.hh"
#include	<stdio.h>
#include	"textures.h"

IDirect3DBaseTexture9*	curTexture;

HRESULT	MyD3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
  if (pTexture == get_textures()[1].pointer)
    wprintf(L"Loading font texture : ");
  else if (curTexture == get_textures()[1].pointer)
    wprintf(L"Unloading font texture : ");
  curTexture = pTexture;
  wprintf(L"SetTexture\n");
  return orig->SetTexture(Stage, pTexture);
}

HRESULT	MyD3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
  wprintf(L"SetRenderState : %d, %d\n", State, Value);
  return orig->SetRenderState(State, Value);
}

HRESULT	MyD3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
  wprintf(L"SetVertexShader : %p\n", pShader);
  return orig->SetVertexShader(pShader);
}

HRESULT	MyD3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  wprintf(L"SetPixelShader : %p\n", pShader);
  return orig->SetPixelShader(pShader);
}

HRESULT	MyD3DDevice9::SetFVF(DWORD FVF)
{
  wprintf(L"SetFVF : %d\n", FVF);
  return orig->SetFVF(FVF);
}



HRESULT	MyD3DDevice9::QueryInterface(REFIID riid, void** ppvObj)
{
  wprintf(L"QueryInterface\n");
  return orig->QueryInterface(riid, ppvObj);
}

ULONG	MyD3DDevice9::AddRef()
{
  wprintf(L"AddRef\n");
  return orig->AddRef();
}

ULONG	MyD3DDevice9::Release()
{
  wprintf(L"Release\n");
  return orig->Release();
}

HRESULT	MyD3DDevice9::TestCooperativeLevel()
{
  wprintf(L"TestCooperativeLevel\n");
  return orig->TestCooperativeLevel();
}

UINT	MyD3DDevice9::GetAvailableTextureMem()
{
  wprintf(L"GetAvailableTextureMem\n");
  return orig->GetAvailableTextureMem();
}

HRESULT	MyD3DDevice9::EvictManagedResources()
{
  wprintf(L"EvictManagedResources\n");
  return orig->EvictManagedResources();
}

HRESULT	MyD3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
  wprintf(L"GetDirect3D\n");
  return orig->GetDirect3D(ppD3D9);
}

HRESULT	MyD3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
  wprintf(L"GetDeviceCaps\n");
  return orig->GetDeviceCaps(pCaps);
}

HRESULT	MyD3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
  wprintf(L"GetDisplayMode\n");
  return orig->GetDisplayMode(iSwapChain, pMode);
}

HRESULT	MyD3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
  wprintf(L"GetCreationParameters\n");
  return orig->GetCreationParameters(pParameters);
}

HRESULT	MyD3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
  wprintf(L"SetCursorProperties\n");
  return orig->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void	MyD3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
{
  wprintf(L"SetCursorPosition\n");
  return orig->SetCursorPosition(X, Y, Flags);
}

BOOL	MyD3DDevice9::ShowCursor(BOOL bShow)
{
  wprintf(L"ShowCursor\n");
  return orig->ShowCursor(bShow);
}

HRESULT	MyD3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
  wprintf(L"CreateAdditionalSwapChain\n");
  return orig->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT	MyD3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
  wprintf(L"GetSwapChain\n");
  return orig->GetSwapChain(iSwapChain, pSwapChain);
}

UINT	MyD3DDevice9::GetNumberOfSwapChains()
{
  wprintf(L"GetNumberOfSwapChains\n");
  return orig->GetNumberOfSwapChains();
}

HRESULT	MyD3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  wprintf(L"Reset\n");
  return orig->Reset(pPresentationParameters);
}

HRESULT	MyD3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
  wprintf(L"Present\n");
  return orig->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT	MyD3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
  wprintf(L"GetBackBuffer\n");
  return orig->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT	MyD3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
  wprintf(L"GetRasterStatus\n");
  return orig->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT	MyD3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
  wprintf(L"SetDialogBoxMode\n");
  return orig->SetDialogBoxMode(bEnableDialogs);
}

void	MyD3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
  wprintf(L"SetGammaRamp\n");
  return orig->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void	MyD3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
  wprintf(L"GetGammaRamp\n");
  return orig->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT	MyD3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
  wprintf(L"CreateTexture\n");
  return orig->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
  wprintf(L"CreateVolumeTexture\n");
  return orig->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
  wprintf(L"CreateCubeTexture\n");
  return orig->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
  wprintf(L"CreateVertexBuffer\n");
  return orig->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
  wprintf(L"CreateIndexBuffer\n");
  return orig->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  wprintf(L"CreateRenderTarget\n");
  return orig->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT	MyD3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  wprintf(L"CreateDepthStencilSurface\n");
  return orig->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT	MyD3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
  wprintf(L"UpdateSurface\n");
  return orig->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT	MyD3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
  wprintf(L"UpdateTexture\n");
  return orig->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT	MyD3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
  wprintf(L"GetRenderTargetData\n");
  return orig->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT	MyD3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
  wprintf(L"GetFrontBufferData\n");
  return orig->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT	MyD3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
  wprintf(L"StretchRect\n");
  return orig->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT	MyD3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
  wprintf(L"ColorFill\n");
  return orig->ColorFill(pSurface, pRect, color);
}

HRESULT	MyD3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  wprintf(L"CreateOffscreenPlainSurface\n");
  return orig->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT	MyD3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
  wprintf(L"SetRenderTarget\n");
  return orig->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT	MyD3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
  wprintf(L"GetRenderTarget\n");
  return orig->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT	MyD3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
  wprintf(L"SetDepthStencilSurface\n");
  return orig->SetDepthStencilSurface(pNewZStencil);
}

HRESULT	MyD3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
  wprintf(L"GetDepthStencilSurface\n");
  return orig->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT	MyD3DDevice9::BeginScene()
{
  wprintf(L"BeginScene\n");
  return orig->BeginScene();
}

HRESULT	MyD3DDevice9::EndScene()
{
  wprintf(L"EndScene\n");
  return orig->EndScene();
}

HRESULT	MyD3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
  wprintf(L"Clear\n");
  return orig->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT	MyD3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
  wprintf(L"SetTransform\n");
  return orig->SetTransform(State, pMatrix);
}

HRESULT	MyD3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
  wprintf(L"GetTransform\n");
  return orig->GetTransform(State, pMatrix);
}

HRESULT	MyD3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE param1, CONST D3DMATRIX* param2)
{
  wprintf(L"MultiplyTransform\n");
  return orig->MultiplyTransform(param1, param2);
}

HRESULT	MyD3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
  wprintf(L"SetViewport\n");
  return orig->SetViewport(pViewport);
}

HRESULT	MyD3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
  wprintf(L"GetViewport\n");
  return orig->GetViewport(pViewport);
}

HRESULT	MyD3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
  wprintf(L"SetMaterial\n");
  return orig->SetMaterial(pMaterial);
}

HRESULT	MyD3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
  wprintf(L"GetMaterial\n");
  return orig->GetMaterial(pMaterial);
}

HRESULT	MyD3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* param2)
{
  wprintf(L"SetLight\n");
  return orig->SetLight(Index, param2);
}

HRESULT	MyD3DDevice9::GetLight(DWORD Index, D3DLIGHT9* param2)
{
  wprintf(L"GetLight\n");
  return orig->GetLight(Index, param2);
}

HRESULT	MyD3DDevice9::LightEnable(DWORD Index, BOOL Enable)
{
  wprintf(L"LightEnable\n");
  return orig->LightEnable(Index, Enable);
}

HRESULT	MyD3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable)
{
  wprintf(L"GetLightEnable\n");
  return orig->GetLightEnable(Index, pEnable);
}

HRESULT	MyD3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane)
{
  wprintf(L"SetClipPlane\n");
  return orig->SetClipPlane(Index, pPlane);
}

HRESULT	MyD3DDevice9::GetClipPlane(DWORD Index, float* pPlane)
{
  wprintf(L"GetClipPlane\n");
  return orig->GetClipPlane(Index, pPlane);
}

HRESULT	MyD3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
  wprintf(L"GetRenderState\n");
  return orig->GetRenderState(State, pValue);
}

HRESULT	MyD3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
  wprintf(L"CreateStateBlock\n");
  return orig->CreateStateBlock(Type, ppSB);
}

HRESULT	MyD3DDevice9::BeginStateBlock()
{
  wprintf(L"BeginStateBlock\n");
  return orig->BeginStateBlock();
}

HRESULT	MyD3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
  wprintf(L"EndStateBlock\n");
  return orig->EndStateBlock(ppSB);
}

HRESULT	MyD3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
  wprintf(L"SetClipStatus\n");
  return orig->SetClipStatus(pClipStatus);
}

HRESULT	MyD3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
  wprintf(L"GetClipStatus\n");
  return orig->GetClipStatus(pClipStatus);
}

HRESULT	MyD3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
  wprintf(L"GetTexture\n");
  return orig->GetTexture(Stage, ppTexture);
}

HRESULT	MyD3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
  wprintf(L"GetTextureStageState\n");
  return orig->GetTextureStageState(Stage, Type, pValue);
}

HRESULT	MyD3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
  wprintf(L"SetTextureStageState\n");
  return orig->SetTextureStageState(Stage, Type, Value);
}

HRESULT	MyD3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
  wprintf(L"GetSamplerState\n");
  return orig->GetSamplerState(Sampler, Type, pValue);
}

HRESULT	MyD3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
  wprintf(L"SetSamplerState\n");
  return orig->SetSamplerState(Sampler, Type, Value);
}

HRESULT	MyD3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
  wprintf(L"ValidateDevice\n");
  return orig->ValidateDevice(pNumPasses);
}

HRESULT	MyD3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
  wprintf(L"SetPaletteEntries\n");
  return orig->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT	MyD3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
  wprintf(L"GetPaletteEntries\n");
  return orig->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT	MyD3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
  wprintf(L"SetCurrentTexturePalette\n");
  return orig->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT	MyD3DDevice9::GetCurrentTexturePalette(UINT* PaletteNumber)
{
  wprintf(L"GetCurrentTexturePalette\n");
  return orig->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT	MyD3DDevice9::SetScissorRect(CONST RECT* pRect)
{
  wprintf(L"SetScissorRect\n");
  return orig->SetScissorRect(pRect);
}

HRESULT	MyD3DDevice9::GetScissorRect(RECT* pRect)
{
  wprintf(L"GetScissorRect\n");
  return orig->GetScissorRect(pRect);
}

HRESULT	MyD3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
  wprintf(L"SetSoftwareVertexProcessing\n");
  return orig->SetSoftwareVertexProcessing(bSoftware);
}

BOOL	MyD3DDevice9::GetSoftwareVertexProcessing()
{
  wprintf(L"GetSoftwareVertexProcessing\n");
  return orig->GetSoftwareVertexProcessing();
}

HRESULT	MyD3DDevice9::SetNPatchMode(float nSegments)
{
  wprintf(L"SetNPatchMode\n");
  return orig->SetNPatchMode(nSegments);
}

float	MyD3DDevice9::GetNPatchMode()
{
  wprintf(L"GetNPatchMode\n");
  return orig->GetNPatchMode();
}

HRESULT	MyD3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
  wprintf(L"DrawPrimitive\n");
  return orig->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT	MyD3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE param1, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
  wprintf(L"DrawIndexedPrimitive\n");
  return orig->DrawIndexedPrimitive(param1, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT	MyD3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  wprintf(L"DrawPrimitiveUP\n");
  return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	MyD3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  wprintf(L"DrawIndexedPrimitiveUP\n");
  return orig->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	MyD3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
  wprintf(L"ProcessVertices\n");
  return orig->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT	MyD3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
  wprintf(L"CreateVertexDeclaration\n");
  return orig->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT	MyD3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
  wprintf(L"SetVertexDeclaration\n");
  return orig->SetVertexDeclaration(pDecl);
}

HRESULT	MyD3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
  wprintf(L"GetVertexDeclaration\n");
  return orig->GetVertexDeclaration(ppDecl);
}

HRESULT	MyD3DDevice9::GetFVF(DWORD* pFVF)
{
  wprintf(L"GetFVF\n");
  return orig->GetFVF(pFVF);
}

HRESULT	MyD3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
  wprintf(L"CreateVertexShader\n");
  return orig->CreateVertexShader(pFunction, ppShader);
}

HRESULT	MyD3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
  wprintf(L"GetVertexShader\n");
  return orig->GetVertexShader(ppShader);
}

HRESULT	MyD3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  wprintf(L"SetVertexShaderConstantF\n");
  return orig->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	MyD3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  wprintf(L"GetVertexShaderConstantF\n");
  return orig->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	MyD3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  wprintf(L"SetVertexShaderConstantI\n");
  return orig->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	MyD3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  wprintf(L"GetVertexShaderConstantI\n");
  return orig->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	MyD3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  wprintf(L"SetVertexShaderConstantB\n");
  return orig->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	MyD3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  wprintf(L"GetVertexShaderConstantB\n");
  return orig->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	MyD3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
  wprintf(L"SetStreamSource\n");
  return orig->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT	MyD3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
  wprintf(L"GetStreamSource\n");
  return orig->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT	MyD3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
  wprintf(L"SetStreamSourceFreq\n");
  return orig->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT	MyD3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
  wprintf(L"GetStreamSourceFreq\n");
  return orig->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT	MyD3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
  wprintf(L"SetIndices\n");
  return orig->SetIndices(pIndexData);
}

HRESULT	MyD3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
  wprintf(L"GetIndices\n");
  return orig->GetIndices(ppIndexData);
}

HRESULT	MyD3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
  wprintf(L"CreatePixelShader\n");
  return orig->CreatePixelShader(pFunction, ppShader);
}

HRESULT	MyD3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
  wprintf(L"GetPixelShader\n");
  return orig->GetPixelShader(ppShader);
}

HRESULT	MyD3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  wprintf(L"SetPixelShaderConstantF\n");
  return orig->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	MyD3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  wprintf(L"GetPixelShaderConstantF\n");
  return orig->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	MyD3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  wprintf(L"SetPixelShaderConstantI\n");
  return orig->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	MyD3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  wprintf(L"GetPixelShaderConstantI\n");
  return orig->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	MyD3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  wprintf(L"SetPixelShaderConstantB\n");
  return orig->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	MyD3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  wprintf(L"GetPixelShaderConstantB\n");
  return orig->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	MyD3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
  wprintf(L"DrawRectPatch\n");
  return orig->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT	MyD3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
  wprintf(L"DrawTriPatch\n");
  return orig->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT	MyD3DDevice9::DeletePatch(UINT Handle)
{
  wprintf(L"DeletePatch\n");
  return orig->DeletePatch(Handle);
}

HRESULT	MyD3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
  wprintf(L"CreateQuery\n");
  return orig->CreateQuery(Type, ppQuery);
}
