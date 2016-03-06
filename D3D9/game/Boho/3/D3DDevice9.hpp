#ifndef BOHO3_D3DDEVICE9_HPP_
# define BOHO3_D3DDEVICE9_HPP_

# include	"Boho/D3DDevice9.hpp"

#define NONE 0
#define SELECT 1

namespace	Boho3
{
  class	D3DDevice9 : public Boho::AD3DDevice9
  {
  private:
    IDirect3DPixelShader9*	curPixelShader;
    bool			is_menu;

    void	handlePixelShader();
    HRESULT	printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);

  public:
    D3DDevice9();
    ~D3DDevice9();

    HRESULT __attribute__((__stdcall__))	SetPixelShader(IDirect3DPixelShader9* pShader);
    HRESULT __attribute__((__stdcall__))	DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT __attribute__((__stdcall__))	Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

    static IDirect3DPixelShader9*	pixelShaderToUse;

    int	state;
  };
}

#endif /* !BOHO3_D3DDEVICE9_HPP_ */
