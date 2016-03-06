#ifndef BOHO1_D3DDEVICE9_HPP_
# define BOHO1_D3DDEVICE9_HPP_

# include	"Boho/D3DDevice9.hpp"

namespace Boho1
{
  class	D3DDevice9 : public Boho::AD3DDevice9
  {
  private:
    HRESULT	printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);

  public:
    D3DDevice9(){}
    ~D3DDevice9(){}
  };
}

#endif /* BOHO1_D3DDEVICE9_HPP_ */
