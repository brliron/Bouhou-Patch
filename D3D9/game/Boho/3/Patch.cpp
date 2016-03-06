#include	"Boho/3/Patch.hpp"
#include	"Boho/3/D3DDevice9.hpp"
#include	"Reader.hpp"


LPCWSTR	Boho3::Patch::getGameName() const
{
  return L"boho3";
}

LPCWSTR	Boho3::Patch::getWindowName() const
{
  return L"Bouhou Koumakyou 2";
}

D3DDevice9*	Boho3::Patch::newD3DDevice() const
{
  return new Boho3::D3DDevice9();
}
