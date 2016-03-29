#ifndef D3DTEXTURESPATCH_HPP_
# define D3DTEXTURESPATCH_HPP_

# include	"game/AD3DPatch.hpp"

/*
** Implements the APatch class for texture dumping / replacing in D3D9-based games.
*/
namespace	D3DTextures
{
  class	Patch : public AD3DPatch
  {
  private:
    ATexturesManager*	newTexturesManager() const;
    ::D3DDevice9*	newD3DDevice() const;
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;

  public:
    Patch();
    bool		isGameExiting(CWPSTRUCT* msgParams) const;
  };
}

#endif /* !D3DTEXTURESPATCH_HPP_ */
