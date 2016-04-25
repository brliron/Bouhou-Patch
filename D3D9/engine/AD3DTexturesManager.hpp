#ifndef AD3D9TEXTURESMANAGER_HPP_
# define AD3D9TEXTURESMANAGER_HPP_

# include	"D3dx9core.h"
# include	"ATexturesManager.hpp"

namespace D3D9
{
  /*
  ** Structure describing a Direct3D9 texture.
  **
  ** - pointer  : the pointer given by Direct3D, containing the texture. You must set it to NULL.
  **
  ** See ATexture for a description of the functions.
  */
  struct	Texture : public ATexture
  {
    LPDIRECT3DBASETEXTURE9	pointer;

    Texture(LPCSTR hash, LPCWSTR filename, unsigned int flags, LPDIRECT3DBASETEXTURE9 pointer = NULL)
      : ATexture(hash, filename, flags), pointer(pointer) {}

    bool	calcHash(char hash[33]) const;
    bool	loadTranslation();
    void	save(LPCWSTR filename) const;
    void*	getPointer() const;

    static void	saveToFile(LPWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture);
  };

  /*
  ** Implements the ATexturesManager class for a game using Direct3D.
  ** TODO: rename to TexturesManager, because it is not abstract.
  */
  class	ATexturesManager : public ::ATexturesManager
  {
  private:
    ATexture*	makeTexture(LPCSTR hash, LPCWSTR filename, unsigned int flags, void* pointer) const;

    // Loads a texture from a file on disk.
    HRESULT	loadFromFile(LPDIRECT3DDEVICE9 pDevice, Texture* texture, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture);
    // Loads a texture from a file in memory.
    HRESULT	loadFromMemory(Texture* texture, LPCSTR hash, LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture);

  public:
    ATexturesManager();
    ~ATexturesManager();

    // Wrapper around D3DXCreateTextureFromFileInMemoryEx
    HRESULT	loadTextureFromMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture);
  };

}
#endif /* !AD3DTEXTURESMANAGER_HPP_ */
