#ifndef TEXTURES_FLAGS_HPP_
# define TEXTURES_FLAGS_HPP_

/*
** Some flags used with the TexturesManager.
**
** CHARACTERS_PICTURES describes the texture containig the characters' faces.
**
** FONT describes the texture containing the bitmap font.
**
** STAGE describes the stages' textures.
** The stage number is stored in the 3rd byte. You can obtain it by doing
** flags & 0x00FF0000.
*/
namespace Boho
{
  namespace TexturesFlags
  {
    static const unsigned int	CHARACTERS_PICTURES	= 0x01000000;
    static const unsigned int	FONT			= 0x02000000;
    static const unsigned int	STAGE			= 0x04000000;
    static const unsigned int	STAGE01			= 0x04010000;
    static const unsigned int	STAGE02			= 0x04020000;
    static const unsigned int	STAGE03			= 0x04030000;
    static const unsigned int	STAGE04			= 0x04040000;
    static const unsigned int	STAGE05			= 0x04050000;
    static const unsigned int	STAGE06			= 0x04060000;
    static const unsigned int	STAGE07			= 0x04070000;
    static const unsigned int	STAGE08			= 0x04080000;
    static const unsigned int	STAGE09			= 0x04090000;
    static const unsigned int	STAGE10			= 0x040A0000;
    static const unsigned int	STAGE11			= 0x040B0000;
  }
}

#endif /* !TEXTURES_FLAGS_HPP_ */
