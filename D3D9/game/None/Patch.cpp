#include	"None/Patch.hpp"
#include	"D3DDevice9.hpp"
#include	"ATexturesManager.hpp"

LPCWSTR	None::Patch::getGameName() const
{
  return L"";
}

LPCWSTR	None::Patch::getWindowName() const
{
  return L"";
}

bool	None::Patch::isGameExiting(CWPSTRUCT*) const
{
  return false;
}
