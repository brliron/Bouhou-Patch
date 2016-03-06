#ifndef BOHO_D3DDEVICE9_HPP_
# define BOHO_D3DDEVICE9_HPP_

# include	"engine/D3DDevice9.hpp"

/*
** Subclass of D3DDevice9 for the Boho games.
** DrawPrimitiveUp is the function used by these games to draw everything.
*/
namespace	Boho
{
  class	AD3DDevice9 : public ::D3DDevice9
  {
  private:
    virtual void	handlePixelShader();
    virtual HRESULT	printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) = 0;

  public:
    AD3DDevice9(){}
    ~AD3DDevice9(){}
    HRESULT __attribute__((__stdcall__))	DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
  };
}

#endif /* !BOHO_D3DDEVICE9_HPP_ */
