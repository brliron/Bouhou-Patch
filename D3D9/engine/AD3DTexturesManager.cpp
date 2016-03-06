#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	"AD3DTexturesManager.hpp"
#include	"AD3DPatch.hpp"
#include	"Reader.hpp"
#include	"Output.hpp"
#include	"BackgroundTasksManager.hpp"
#include	"md5.hpp"

bool	D3D9::Texture::calcHash(char hash[33]) const
{
  LPD3DXBUFFER	raw_bmp;

  if (D3DXSaveTextureToFileInMemory(&raw_bmp, D3DXIFF_BMP, this->pointer, NULL) != D3D_OK)
    return false;
  md5(raw_bmp->GetBufferPointer(), raw_bmp->GetBufferSize(), hash);
  return true;
}

void	D3D9::Texture::save(LPCWSTR filename) const
{
  this->pointer->AddRef(); // Be careful : a game may use IDirect3DResource9::FreePrivateData, which may invalidate our texture.
  BackgroundTasksManager::get().add(D3D9::Texture::saveToFile, wcsdup(filename), D3DXIFF_PNG, this->pointer);
}

void	D3D9::Texture::saveToFile(LPWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture)
{
  D3DXSaveTextureToFileW(pDestFile, DestFormat, pSrcTexture, NULL);
  endManager->addFile(pDestFile);
  pSrcTexture->Release();
  free(pDestFile);
}

void*	D3D9::Texture::getPointer() const
{
  return this->pointer;
}




D3D9::ATexturesManager::ATexturesManager()
{}

D3D9::ATexturesManager::~ATexturesManager()
{}



ATexture*	D3D9::ATexturesManager::makeTexture(LPCSTR hash, LPCWSTR filename, unsigned int flags, void* pointer) const
{
  return new D3D9::Texture(hash, filename, flags, (LPDIRECT3DTEXTURE9)pointer);
}



HRESULT		D3D9::ATexturesManager::loadFromFile(LPDIRECT3DDEVICE9 pDevice, Texture* texture, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  HRESULT	res;
  LPCWSTR	path;

  path = Reader::get().getFilePath(texture->filename);
  Output::printf(L"from file %s.\n", path);
  res = D3DXCreateTextureFromFileExW(pDevice, path, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  if (res != D3D_OK)
    {
      Output::printf(L"Error : could not load texture : error %u. Loading ", res);
      return res;
    }
  texture->pointer = *ppTexture;
  return res;
}

HRESULT		D3D9::ATexturesManager::loadFromMemory(Texture* texture, LPCSTR hash, LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  WCHAR		filename[MAX_PATH];
  HRESULT	res;

  Output::write(L"from memory.\n");
  res = D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  if (texture)
    texture->pointer = *ppTexture;
  if (!texture || (texture->filename && endManager->isFileInArchive(texture->filename) == false))
    {
      if (texture && texture->filename)
        {
	  Output::write(L"Saving texture.\n");
	  texture->save(texture->filename);
	}
      else
        {
	  swprintf(filename, L"%S.png", hash);
	  if (endManager->isFileInArchive(filename) == false)
	    {
	      Output::write(L"Saving texture.\n");
	      D3D9::Texture(NULL, NULL, 0, *ppTexture).save(filename);
	    }
	}
    }
  return res;
}

HRESULT		D3D9::ATexturesManager::loadTextureFromMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  HRESULT	res;
  char		hash[33];
  Texture*	texture;
  unsigned int	i;

  md5(pSrcData, SrcDataSize, hash);

  i = 0;
  while (i < this->textures.size() && strcmp(hash, this->textures[i]->hash) != 0)
    i++;
  if (i < this->textures.size())
    texture = dynamic_cast<D3D9::Texture*>(this->textures[i]);
  else
    texture = NULL;

  Output::printf(L"Loading texture %S (filename: %s) ", hash, texture && texture->filename ? texture->filename : L"none");

  this->textureLoaded(texture);

  res = D3DERR_NOTAVAILABLE;
  if (texture && texture->filename)
    res = this->loadFromFile(pDevice, texture, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  if (res != D3D_OK)
    res = this->loadFromMemory(texture, hash, pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  return res;
}




extern "C" HRESULT WINAPI	TexturesManager_loadTextureFromMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  if (AD3DPatch::hasInstance())
    return AD3DPatch::get()->getD3D9TexturesManager()->loadTextureFromMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  else
    return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
}
