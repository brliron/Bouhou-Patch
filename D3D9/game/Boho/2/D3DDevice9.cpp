#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	"Boho/2/D3DDevice9.hpp"
#include	"Log.hpp"

IDirect3DPixelShader9*	Boho2::D3DDevice9::pixelShaderToUse = NULL;

HRESULT		Boho2::D3DDevice9::printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  TLVERTEX*	tab;
  WCHAR		str[2048];
  const WCHAR*	name;

  tab = (TLVERTEX*) pVertexStreamZeroData;
  // if (float_cmp(tab[0].v, 0.882813) && float_cmp(tab[0].u, 0.062500) == false)
  if (0.273438 < tab[0].u && tab[0].u < 0.75 && tab[0].v < 0.843750 && float_cmp(tab[0].u, 0.5) == false)
    {
/*      if (float_cmp(tab[0].u, 0.378906))
	name = L"Meiling";
      else if (float_cmp(tab[0].u, 0.503906))
	name = L"Patchouli";
      else if (float_cmp(tab[0].u, 0.628906))
	name = L"Sakuya";
      else if (float_cmp(tab[0].u, 0.753906))
	name = L"Remilia";
      else if (float_cmp(tab[0].u, 0.878906))
	name = L"Flandre";
      else*/
	name = L"An unknown character";
      swprintf(str, L"#  %s is talking (u=%f, v=%f)\n", name, tab[0].u, tab[0].v);
      // swprintf(str, L"#  %s :\n", name);
      my_log->put_text(str);
    }
  return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	Boho2::D3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  this->curPixelShader = pShader;
  return orig->SetPixelShader(pShader);
}

void	Boho2::D3DDevice9::handlePixelShader()
{
  Boho2::D3DDevice9::pixelShaderToUse = curPixelShader;
}

