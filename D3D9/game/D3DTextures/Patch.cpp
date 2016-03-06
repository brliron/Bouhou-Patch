#include	"D3DTextures/Patch.hpp"

namespace D3DTextures
{
  class	CharBuff : public ACharBuff
  {
    void	put_char(const Char&, int, int, int, int){}
    void	put_char(const Char&, int, int){}
    void	put_char(const DisplayedChar&){}
  };
}

D3DTextures::Patch::Patch()
{}

ACharBuff*	D3DTextures::Patch::newCharBuff(int) const
{
  return new D3DTextures::CharBuff();
}

ATexturesManager*	D3DTextures::Patch::newTexturesManager() const
{
  return new D3D9::ATexturesManager();
}

D3DDevice9*	D3DTextures::Patch::newD3DDevice() const
{
  return new D3DDevice9();
}

LPCWSTR	D3DTextures::Patch::getGameName() const
{
  return L"D3DTextures";
}

LPCWSTR	D3DTextures::Patch::getWindowName() const
{
  return L"Placeholder window name";
}

bool	D3DTextures::Patch::isGameExiting(CWPSTRUCT*) const
{
  return false;
}
