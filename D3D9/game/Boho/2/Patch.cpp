#include	"Boho/2/Patch.hpp"
#include	"Boho/2/D3DDevice9.hpp"
#include	"Reader.hpp"


LPCWSTR	Boho2::Patch::getGameName() const
{
  return L"boho2";
}

LPCWSTR	Boho2::Patch::getWindowName() const
{
  return L"Bouhou Youyoumu";
}

D3DDevice9*	Boho2::Patch::newD3DDevice() const
{
  return new Boho2::D3DDevice9();
}
