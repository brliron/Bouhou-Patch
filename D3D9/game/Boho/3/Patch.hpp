#ifndef BOHO3PATCH_HPP_
# define BOHO3PATCH_HPP_

# include	"Boho/APatch.hpp"

namespace Boho3
{
  class	Patch : public Boho::APatch
  {
  private:
    ::D3DDevice9*	newD3DDevice() const;
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;
  };
}

#endif /* !BOHO3PATCH_HPP_ */
