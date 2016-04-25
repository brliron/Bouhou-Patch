#include	"D3DTextures/Patch.hpp"

D3DTextures::Patch::Patch()
{}

ATexturesManager*	D3DTextures::Patch::newTexturesManager() const
{
  return new D3D9::ATexturesManager();
}

ACharBuff*	D3DTextures::Patch::newCharBuff(int) const
{
  return new ACharBuff();
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
