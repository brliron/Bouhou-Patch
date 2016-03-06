#include	"None/Patch.hpp"
#include	"D3DDevice9.hpp"
#include	"ATexturesManager.hpp"

namespace None
{
  class	CharBuff : public ACharBuff
  {
    void	put_char(const Char&, int, int, int, int){}
    void	put_char(const Char&, int, int){}
    void	put_char(const DisplayedChar&){}
  };

  class	TexturesManager : public ATexturesManager
  {
    ATexture*	makeTexture(LPCSTR, LPCWSTR, unsigned int, void*) const{return NULL;}
  };
}

ATexturesManager*	None::Patch::newTexturesManager() const
{
  return new None::TexturesManager();
}

ACharBuff*	None::Patch::newCharBuff(int) const
{
  return new None::CharBuff();
}

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
