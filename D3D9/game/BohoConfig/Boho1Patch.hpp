#ifndef BOHO1CONFIG_PATCH_HPP_
# define BOHO1CONFIG_PATCH_HPP_

# include	"BohoConfig/APatch.hpp"

namespace BohoConfig
{
  class	Boho1Patch : public BohoConfig::APatch
  {
  private:
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;
  };
}

#endif /* !BOHO1CONFIG_PATCH_HPP_ */
