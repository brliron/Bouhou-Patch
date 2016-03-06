#ifndef BOHOPATCH_HPP_
# define BOHOPATCH_HPP_

# include	"game/AD3DPatch.hpp"
# include	"Boho/D3DDevice9.hpp"

/*
** Implements a part of the APatch class for the Boho games (Boho Koumakyou, Boho Youyoumu and Boho Koumakyou 2).
** Each game must complete this interface.
*/
namespace	Boho
{
  class	APatch : public AD3DPatch
  {
  private:
    ACharBuff*		newCharBuff(int flags = CHARBUFF__NO_FLAGS) const;
    ACharBuff*		newCharBuff(Boho::AD3DDevice9* d3dd) const;
    ATexturesManager*	newTexturesManager() const;

  public:
    APatch();
    void		initD3D(HWND hFocusWindow, D3DPRESENT_PARAMETERS* pPresentationParameters);
    ACharBuff*		storeNewCharBuff(Boho::AD3DDevice9* d3dd);
    bool		isGameExiting(CWPSTRUCT* msgParams) const;
  };
}

#endif /* !BOHOPATCH_HPP_ */
