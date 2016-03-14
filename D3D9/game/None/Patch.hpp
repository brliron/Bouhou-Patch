#ifndef NONE_PATCH_HPP_
# define NONE_PATCH_HPP_

# include	"game/APatch.hpp"

// Implements the APatch class for an unknown game.
namespace	None
{
  class	Patch : public ::APatch
  {
  private:
    LPCWSTR		getGameName() const;
    LPCWSTR		getWindowName() const;

  public:
    bool		isGameExiting(CWPSTRUCT* msgParams) const;
  };
}

#endif /* !NONE_PATCH_HPP_ */
