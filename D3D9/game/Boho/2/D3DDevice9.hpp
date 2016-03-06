#ifndef BOHO2_D3DDEVICE9_HPP_
# define BOHO2_D3DDEVICE9_HPP_

# include	"Boho/D3DDevice9.hpp"

namespace Boho2
{
  class	D3DDevice9 : public Boho::AD3DDevice9
  {
  private:
    IDirect3DPixelShader9*	curPixelShader;

    void	handlePixelShader();
    HRESULT	printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);

  public:
    D3DDevice9(){}
    ~D3DDevice9(){}

    HRESULT __attribute__((__stdcall__))	SetPixelShader(IDirect3DPixelShader9* pShader);

    static IDirect3DPixelShader9*	pixelShaderToUse;
  };
}

#endif /* BOHO2_D3DDEVICE9_HPP_ */
