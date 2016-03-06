#include	"D3DDevice9.hpp"
#include	"APatch.hpp"
#include	"Reader.hpp"
#include	"chars.hpp"


D3DDevice9::D3DDevice9()
{}

D3DDevice9::~D3DDevice9()
{}

HRESULT	D3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
  static int	saveEmptyTextures = -1;

  if (saveEmptyTextures == -1)
    saveEmptyTextures = Reader::get().iniGetBool(L"global", L"SAVE_EMPTY_TEXTURES") ? 1 : 0;
  if (saveEmptyTextures == 1 && APatch::get().getCharBuff().empty() == false)
    this->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(1, 0, 0, 0), 0, 0);

  APatch::get().getCharBuff().dump_str();
  if (APatch::get().getTexturesManager().getIdx(pTexture) == -1)
    APatch::get().getTexturesManager().addTexture(pTexture);
  this->curTexture = pTexture;
  return this->orig->SetTexture(Stage, pTexture);
}


/*
** All the following functions do nothing more than calling the original functions.
*/

HRESULT	D3DDevice9::QueryInterface(REFIID riid, void** ppvObj)
{
  return orig->QueryInterface(riid, ppvObj);
}

ULONG	D3DDevice9::AddRef()
{
  return orig->AddRef();
}

ULONG	D3DDevice9::Release()
{
  return orig->Release();
}

HRESULT	D3DDevice9::EndScene()
{
  return orig->EndScene();
}

HRESULT	D3DDevice9::TestCooperativeLevel()
{
  return orig->TestCooperativeLevel();
}

UINT	D3DDevice9::GetAvailableTextureMem()
{
  return orig->GetAvailableTextureMem();
}

HRESULT	D3DDevice9::EvictManagedResources()
{
  return orig->EvictManagedResources();
}

HRESULT	D3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
  return orig->GetDirect3D(ppD3D9);
}

HRESULT	D3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
  return orig->GetDeviceCaps(pCaps);
}

HRESULT	D3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
  return orig->GetDisplayMode(iSwapChain, pMode);
}

HRESULT	D3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
  return orig->GetCreationParameters(pParameters);
}

HRESULT	D3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
  return orig->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void	D3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
{
  return orig->SetCursorPosition(X, Y, Flags);
}

BOOL	D3DDevice9::ShowCursor(BOOL bShow)
{
  return orig->ShowCursor(bShow);
}

HRESULT	D3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
  return orig->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT	D3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
  return orig->GetSwapChain(iSwapChain, pSwapChain);
}

UINT	D3DDevice9::GetNumberOfSwapChains()
{
  return orig->GetNumberOfSwapChains();
}

HRESULT	D3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  return orig->Reset(pPresentationParameters);
}

HRESULT	D3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
  return orig->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT	D3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
  return orig->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT	D3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
  return orig->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT	D3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
  return orig->SetDialogBoxMode(bEnableDialogs);
}

void	D3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
  return orig->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void	D3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
  return orig->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT	D3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
  return orig->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT	D3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
  return orig->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT	D3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
  return orig->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT	D3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
  return orig->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT	D3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
  return orig->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT	D3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return orig->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT	D3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return orig->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT	D3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
  return orig->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT	D3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
  return orig->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT	D3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
  return orig->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT	D3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
  return orig->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT	D3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
  return orig->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT	D3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
  return orig->ColorFill(pSurface, pRect, color);
}

HRESULT	D3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return orig->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT	D3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
  return orig->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT	D3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
  return orig->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT	D3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
  return orig->SetDepthStencilSurface(pNewZStencil);
}

HRESULT	D3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
  return orig->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT	D3DDevice9::BeginScene()
{
  return orig->BeginScene();
}

HRESULT	D3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
  return orig->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT	D3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
  return orig->SetTransform(State, pMatrix);
}

HRESULT	D3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
  return orig->GetTransform(State, pMatrix);
}

HRESULT	D3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
  return orig->MultiplyTransform(State, pMatrix);
}

HRESULT	D3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
  return orig->SetViewport(pViewport);
}

HRESULT	D3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
  return orig->GetViewport(pViewport);
}

HRESULT	D3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
  return orig->SetMaterial(pMaterial);
}

HRESULT	D3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
  return orig->GetMaterial(pMaterial);
}

HRESULT	D3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
  return orig->SetLight(Index, pLight);
}

HRESULT	D3DDevice9::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
  return orig->GetLight(Index, pLight);
}

HRESULT	D3DDevice9::LightEnable(DWORD Index, BOOL Enable)
{
  return orig->LightEnable(Index, Enable);
}

HRESULT	D3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable)
{
  return orig->GetLightEnable(Index, pEnable);
}

HRESULT	D3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane)
{
  return orig->SetClipPlane(Index, pPlane);
}

HRESULT	D3DDevice9::GetClipPlane(DWORD Index, float* pPlane)
{
  return orig->GetClipPlane(Index, pPlane);
}

HRESULT	D3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
  return orig->SetRenderState(State, Value);
}

HRESULT	D3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
  return orig->GetRenderState(State, pValue);
}

HRESULT	D3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
  return orig->CreateStateBlock(Type, ppSB);
}

HRESULT	D3DDevice9::BeginStateBlock()
{
  return orig->BeginStateBlock();
}

HRESULT	D3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
  return orig->EndStateBlock(ppSB);
}

HRESULT	D3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
  return orig->SetClipStatus(pClipStatus);
}

HRESULT	D3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
  return orig->GetClipStatus(pClipStatus);
}

HRESULT	D3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
  return orig->GetTexture(Stage, ppTexture);
}

HRESULT	D3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
  return orig->GetTextureStageState(Stage, Type, pValue);
}

HRESULT	D3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
  return orig->SetTextureStageState(Stage, Type, Value);
}

HRESULT	D3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
  return orig->GetSamplerState(Sampler, Type, pValue);
}

HRESULT	D3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
  return orig->SetSamplerState(Sampler, Type, Value);
}

HRESULT	D3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
  return orig->ValidateDevice(pNumPasses);
}

HRESULT	D3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
  return orig->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT	D3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
  return orig->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT	D3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
  return orig->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT	D3DDevice9::GetCurrentTexturePalette(UINT* PaletteNumber)
{
  return orig->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT	D3DDevice9::SetScissorRect(CONST RECT* pRect)
{
  return orig->SetScissorRect(pRect);
}

HRESULT	D3DDevice9::GetScissorRect(RECT* pRect)
{
  return orig->GetScissorRect(pRect);
}

HRESULT	D3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
  return orig->SetSoftwareVertexProcessing(bSoftware);
}

BOOL	D3DDevice9::GetSoftwareVertexProcessing()
{
  return orig->GetSoftwareVertexProcessing();
}

HRESULT	D3DDevice9::SetNPatchMode(float nSegments)
{
  return orig->SetNPatchMode(nSegments);
}

float	D3DDevice9::GetNPatchMode()
{
  return orig->GetNPatchMode();
}

HRESULT	D3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
  return orig->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT	D3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
  return orig->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT	D3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	D3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  return orig->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	D3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
  return orig->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT	D3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
  return orig->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT	D3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
  return orig->SetVertexDeclaration(pDecl);
}

HRESULT	D3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
  return orig->GetVertexDeclaration(ppDecl);
}

HRESULT	D3DDevice9::SetFVF(DWORD FVF)
{
  return orig->SetFVF(FVF);
}

HRESULT	D3DDevice9::GetFVF(DWORD* pFVF)
{
  return orig->GetFVF(pFVF);
}

HRESULT	D3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
  return orig->CreateVertexShader(pFunction, ppShader);
}

HRESULT	D3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
  return orig->SetVertexShader(pShader);
}

HRESULT	D3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
  return orig->GetVertexShader(ppShader);
}

HRESULT	D3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  return orig->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	D3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  return orig->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	D3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  return orig->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	D3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  return orig->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	D3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  return orig->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	D3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  return orig->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	D3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
  return orig->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT	D3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
  return orig->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT	D3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
  return orig->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT	D3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
  return orig->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT	D3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
  return orig->SetIndices(pIndexData);
}

HRESULT	D3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
  return orig->GetIndices(ppIndexData);
}

HRESULT	D3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
  return orig->CreatePixelShader(pFunction, ppShader);
}

HRESULT	D3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  return orig->SetPixelShader(pShader);
}

HRESULT	D3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
  return orig->GetPixelShader(ppShader);
}

HRESULT	D3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  return orig->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	D3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  return orig->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT	D3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  return orig->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	D3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  return orig->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT	D3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  return orig->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	D3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  return orig->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT	D3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
  return orig->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT	D3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
  return orig->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT	D3DDevice9::DeletePatch(UINT Handle)
{
  return orig->DeletePatch(Handle);
}

HRESULT	D3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
  return orig->CreateQuery(Type, ppQuery);
}
