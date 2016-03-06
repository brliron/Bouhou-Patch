#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	"Boho/3/D3DDevice9.hpp"
#include	"Boho/TexturesFlags.hpp"
#include	"Log.hpp"
#include	"ATexturesManager.hpp"

IDirect3DPixelShader9*	Boho3::D3DDevice9::pixelShaderToUse = NULL;

Boho3::D3DDevice9::D3DDevice9()
  : curPixelShader(NULL), is_menu(false)
{}

Boho3::D3DDevice9::~D3DDevice9()
{}

HRESULT		Boho3::D3DDevice9::printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  TLVERTEX*	tab;
  WCHAR		str[2048];

  tab = (TLVERTEX*)pVertexStreamZeroData;
  swprintf(str, L"#  %s is talking (u=%f, v=%f)\n", L"Someone", tab[0].u, tab[0].v);
  my_log->put_text(str);
  return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	Boho3::D3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  this->curPixelShader = pShader;
  return orig->SetPixelShader(pShader);
}

void	Boho3::D3DDevice9::handlePixelShader()
{
  Boho3::D3DDevice9::pixelShaderToUse = curPixelShader;
}

HRESULT	Boho3::D3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  TLVERTEX*	tab;

  tab = (TLVERTEX*)pVertexStreamZeroData;
  if (texture_has_flag(this->curTexture, ATexturesManager::COPY) &&
      float_cmp(tab->y, 71.5))
    this->is_menu = true;
  if (this->is_menu &&
      texture_has_flag(this->curTexture, Boho::TexturesFlags::FONT) &&
      80 <= tab->y && tab->y <= 90)
    return this->::D3DDevice9::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
  return this->Boho::AD3DDevice9::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	Boho3::D3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
  this->is_menu = false;
  return this->Boho::AD3DDevice9::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
