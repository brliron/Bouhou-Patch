#ifndef BOHO1PATCH_HPP_
# define BOHO1PATCH_HPP_

# include	"Boho/APatch.hpp"

namespace Boho1
{
  class	Patch : public Boho::APatch
  {
  private:
    ::D3DDevice9*	newD3DDevice() const;
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;
  };
}

#endif /* !BOHO1PATCH_HPP_ */
