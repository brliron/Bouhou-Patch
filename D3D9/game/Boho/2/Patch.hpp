#ifndef BOHO2PATCH_HPP_
# define BOHO2PATCH_HPP_

# include	"Boho/APatch.hpp"

namespace Boho2
{
  class	Patch : public Boho::APatch
  {
  private:
    ::D3DDevice9*	newD3DDevice() const;
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;
  };
}

#endif /* !BOHO2PATCH_HPP_ */
