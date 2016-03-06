#include	"Boho/1/Patch.hpp"
#include	"Boho/1/D3DDevice9.hpp"
#include	"Reader.hpp"


LPCWSTR	Boho1::Patch::getGameName() const
{
  return L"boho1";
}

LPCWSTR	Boho1::Patch::getWindowName() const
{
  return L"Bouhou Koumakyou ~ Scarlet Family ~";
}

D3DDevice9*	Boho1::Patch::newD3DDevice() const
{
  return new Boho1::D3DDevice9();
}
