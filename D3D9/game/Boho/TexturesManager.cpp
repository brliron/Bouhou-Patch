#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	"Boho/TexturesManager.hpp"
#include	"Boho/TexturesFlags.hpp"
#include	"Log.hpp"

void	Boho::TexturesManager::textureLoaded(ATexture* texture)
{
  WCHAR	str[255];

  if (texture && texture->flags & Boho::TexturesFlags::STAGE)
    {
      swprintf(str, L"## Loading stage %d\n", (texture->flags >> 16) & 0xFF);
      log__put_text(str);
    }
}
