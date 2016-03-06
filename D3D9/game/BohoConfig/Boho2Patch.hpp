#ifndef BOHO2CONFIG_PATCH_HPP_
# define BOHO2CONFIG_PATCH_HPP_

# include	"BohoConfig/APatch.hpp"

namespace BohoConfig
{
  class	Boho2Patch : public BohoConfig::APatch
  {
  private:
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;
  };
}

#endif /* !BOHO2CONFIG_PATCH_HPP_ */
