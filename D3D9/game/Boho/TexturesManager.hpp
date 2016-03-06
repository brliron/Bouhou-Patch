#ifndef BOHOTEXTURESMANAGER_HPP_
# define BOHOTEXTURESMANAGER_HPP_

# include	"AD3DTexturesManager.hpp"

namespace	Boho
{
  class	TexturesManager : public D3D9::ATexturesManager
  {
  public:
    void	textureLoaded(ATexture* texture);
  };
}

#endif /* !BOHOTEXTURESMANAGER_HPP_ */
