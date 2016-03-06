#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<Windows.h>
#include	<Shlwapi.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"ATexturesManager.hpp"
#include	"EndManager.hpp"
#include	"APatch.hpp"
#include	"Reader.hpp"
#include	"md5.hpp"


ATexturesManager::ATexturesManager()
{}

ATexturesManager::~ATexturesManager()
{}



void	ATexturesManager::addTexture(ATexture* texture)
{
  this->textures.push_back(texture);
}

void		ATexturesManager::addTexture(LPCSTR hash, LPCWSTR filename, unsigned int flags, void* pointer)
{
  this->addTexture(this->makeTexture(hash, filename, flags, pointer));
}

bool		ATexturesManager::addTexture(void* pointer)
{
  ATexture*	texture = this->makeTexture(NULL, NULL, 0, pointer);
  char		hash[33];
  unsigned int	i;

  if (texture->calcHash(hash) == false)
    {
      delete texture;
      return false;
    }
  for (i = 0; i < this->textures.size(); i++)
    if (strcmp(hash, this->textures[i]->hash) == 0)
      break ;
  if (i < this->textures.size())
    {
      texture->hash     = this->textures[i]->hash;
      texture->filename = this->textures[i]->filename;
      texture->flags    = this->textures[i]->flags;
    }
  else
    {
      if (Reader::get().iniGetBool(L"global", L"SAVE_EMPTY_TEXTURES"))
	{
	  WCHAR	filename[MAX_PATH];
	  swprintf(filename, L"%S.png", hash);
	  if (endManager->isFileInArchive(filename) == false)
	    texture->save(filename);
	}
      texture->hash = strdup(hash);
    }
  texture->flags |= ATexturesManager::COPY;
  this->addTexture(texture);
  return true;
}

void		ATexturesManager::loadTextures()
{
  Reader&	reader = Reader::get();
  LPWSTR	keysPath;
  LPWSTR	keysPathPtr;
  unsigned int	keysPathSize;

  keysPath = NULL;
  keysPathSize = 1;
  do
    {
      keysPathSize += 33 * 8;
      keysPath = (LPWSTR)realloc(keysPath, keysPathSize * sizeof(WCHAR));
    } while (GetPrivateProfileStringW(L"Textures", NULL, NULL, keysPath, keysPathSize, reader.getFilePath(L"config.ini")) == keysPathSize - 2);
  for (keysPathPtr = keysPath; *keysPathPtr != L'\0'; keysPathPtr += wcslen(keysPathPtr) + 1)
    {
      LPCWSTR		val = reader.iniGetString(L"Textures", keysPathPtr);
      char*		hash;
      unsigned int	flags = wcstol(val, NULL, 0);
      LPCWSTR		filename;

      hash = (char*)malloc(33);
      for (int i = 0; i < 33; i++)
	hash[i] = keysPathPtr[i];
      
      if (wcschr(val, L':'))
	val = wcschr(val, L':') + 1;
      else
	val = L"";
      if (*val)
	filename = wcsdup(val);
      else
	filename = NULL;
      this->addTexture(hash, filename, flags, NULL);
    }
  free(keysPath);
}




void*	ATexturesManager::operator[](int idx)
{
  return this->textures[idx]->getPointer();
}

void*	get_texture_by_idx(int idx)
{
  return APatch::get().getTexturesManager()[idx];
}



bool	ATexturesManager::hasFlag(void* texture, unsigned int flag)
{
  for (const ATexture* it : this->textures)
    if (texture == it->getPointer())
      return it->flags & flag;
  return false;
}

bool	texture_has_flag(void* texture, unsigned int flag)
{
  return APatch::get().getTexturesManager().hasFlag(texture, flag);
}



void*	ATexturesManager::getByFlag(unsigned int flag)
{
  for (const ATexture* it : this->textures)
    if (it->flags & flag)
      return it->getPointer();
  return NULL;
}

void*	get_texture_by_flag(unsigned int flag)
{
  return APatch::get().getTexturesManager().getByFlag(flag);
}



int	ATexturesManager::getIdx(void* texture)
{
  for (unsigned int i = 0; i < this->textures.size(); i++)
    if (this->textures[i]->getPointer() == texture)
      return i;
  return -1;
}
