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

D3D9::Texture::Texture(LPCSTR hash, LPCWSTR filename, unsigned int flags, LPDIRECT3DBASETEXTURE9 pointer)
  : ATexture(hash, filename, flags), pointer(pointer)
{
  if (pointer)
    pointer->AddRef();
}

bool	D3D9::Texture::calcHash(char hash[33]) const
{
  LPD3DXBUFFER	raw_bmp;

  if (D3DXSaveTextureToFileInMemory(&raw_bmp, D3DXIFF_BMP, this->pointer, NULL) != D3D_OK)
    return false;
  md5(raw_bmp->GetBufferPointer(), raw_bmp->GetBufferSize(), hash);
  return true;
}

bool	D3D9::Texture::loadTranslation()
{
  if (this->filename == nullptr || this->pointer != nullptr)
    return false;
  LPCWSTR path = Reader::get().getFilePath(this->filename);
  Output::printf(L"Loading translation for texture %S from %s\n", this->hash, path);
  HRESULT ret = D3DXCreateTextureFromFileW(AD3DPatch::get()->getD3DDevice()->orig, path, (LPDIRECT3DTEXTURE9*)&this->pointer);
  if (ret == D3D_OK)
    this->pointer->AddRef();
  else
    {
      Output::write(L"Loading failed.\n");
      this->pointer = nullptr;
    }
  return ret == D3D_OK;
}

void	D3D9::Texture::save(LPCWSTR filename) const
{
  this->pointer->AddRef();
  BackgroundTasksManager::get().add(D3D9::Texture::saveToFile, wcsdup(filename), D3DXIFF_PNG, this->pointer);
}

void	D3D9::Texture::saveToFile(LPWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture)
{
  D3DXSaveTextureToFileW(pDestFile, DestFormat, pSrcTexture, NULL);
  endManager->addFile(pDestFile);
  pSrcTexture->Release();
  free(pDestFile);
}

const Bitmap*	D3D9::Texture::saveToMemory()
{
  if (this->cache_bmp)
    return this->cache_bmp;

  LPD3DXBUFFER	raw_bmp;
  Bitmap*	bmp;

  if (D3DXSaveTextureToFileInMemory(&raw_bmp, D3DXIFF_BMP, this->pointer, NULL) == D3D_OK)
    {
      bmp = new Bitmap;
      if (bmp->load_from_memory(static_cast<unsigned char*>(raw_bmp->GetBufferPointer())))
	{
	  this->cache_bmp = bmp;
	  return bmp;
	}
      delete bmp;
      // TODO: free raw_bmp
    }
  Output::printf(L"Warning : couldn't save texture %S to memory.\n", this->hash);
  return nullptr;
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
  texture->pointer->AddRef();
  return res;
}

HRESULT		D3D9::ATexturesManager::loadFromMemory(Texture* texture, LPCSTR hash, LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  WCHAR		filename[MAX_PATH];
  HRESULT	res;

  Output::write(L"from memory.\n");
  res = D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  if (texture)
    {
      texture->pointer = *ppTexture;
      texture->pointer->AddRef();
    }
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


void	D3D9::ATexturesManager::freeUnusedTextures()
{
  for (unsigned int i = 0; i < this->textures.size(); )
    {
      D3D9::Texture*	texture = static_cast<D3D9::Texture*>(this->textures[i]);
      if (!texture || !texture->pointer)
	{
	  i++;
	  continue ;
	}
      if (texture->pointer->Release() == 0)
	{
	  texture->pointer = nullptr;
	  delete texture;
	  this->textures.erase(this->textures.begin() + i);
	}
      else
	{
	  texture->pointer->AddRef();
	  i++;
	}
    }
}




extern "C" HRESULT WINAPI	TexturesManager_loadTextureFromMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
  if (AD3DPatch::hasInstance())
    return AD3DPatch::get()->getD3D9TexturesManager()->loadTextureFromMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
  else
    return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
}
