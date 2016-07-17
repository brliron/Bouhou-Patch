#ifndef ATEXTURESMANAGER_HPP_
# define ATEXTURESMANAGER_HPP_

# include	<Windows.h>
# include	<vector>

/*
** Structure describing a texture.
** - filename : if not null, the original font will be replaced by the one given here.
**              The path must be relative to the translations directory.
**              If the file doesn't exists, the texture will be saved to this file.
** - hash     : a MD5 hash of the texture. It stored as a string of 33 characters :
**              16 pairs of hexadecimal numbers (with lowercase letters), and a \0.
** - flags    : A bitmask of flags containing various informations (for now, the only existing flag is COPY).
**              Each patch can also define its own flags to identify some textures.
*/
class		ATexture
{
public:
  LPCSTR	hash;
  LPCWSTR	filename;
  unsigned int	flags;
  ATexture*	replacement;

  ATexture(LPCSTR hash, LPCWSTR filename, unsigned int flags)
    : hash(hash), filename(filename), flags(flags), replacement(nullptr) {}
  virtual ~ATexture() {};

  // Computes the texture's hash.
  virtual bool	calcHash(char hash[33]) const = 0;
  // If the filename member of the texture is not empty, load the texture it points to.
  virtual bool	loadTranslation() = 0;
  // Saves the texture to a file.
  virtual void	save(LPCWSTR filename) const = 0;
  // Returns a pointer to the structure the graphic library uses internally to store the texture.
  virtual void*	getPointer() const = 0;
};

/*
** Keeps a list of the textures used by the game.
** A game must implement this class, and fill the textures array in its constructor.
** It can also overwrite the textureLoaded function, which will be called each time the game loads a texture.
**
** I use a void* as the texture pointer used internally by the library (LPDIRECT3DBASETEXTURE9 for Direct3D9, for example).
** If someone has a better idea, I take it.
*/
class			ATexturesManager
{
public:
  // This flag is set if the texture is created by copying another one.
  static const unsigned int	COPY = 0x00000001;

protected:
  std::vector<ATexture*>	textures;

  // Allocates a new texture.
  virtual ATexture*	makeTexture(LPCSTR hash, LPCWSTR filename, unsigned int flags, void* pointer) const = 0;

  void			addTexture(ATexture* texture);
  void			addTexture(LPCSTR hash, LPCWSTR filename, unsigned int flags, void* pointer);

  // Function called each time the textures manager loads a new texture.
  virtual void		textureLoaded(ATexture*){};

public:
  ATexturesManager();
  virtual ~ATexturesManager();

  bool	addTexture(void* pointer);
  void	loadTextures();

  // Returns the internal pointer of the texture having the corresponding index or flag.
  void*	operator[](int idx);
  void*	getByFlag(unsigned int flag);
  // Returns the index of the given texture (-1 if the texture isn't found).
  int	getIdx(void* texture);
  // Returns the replacement texture of the given texture (nullptr if the texture isn't found).
  void*	getReplacement(void* texture);
  // Returns true if the given texture has the given flag, false otherwise.
  bool	hasFlag(void* texture, unsigned int flag);
};


# ifdef __cplusplus
extern "C" {
# endif

  // Returns the internal pointer of the texture having the corresponding index or flag.
  void*	get_texture_by_idx(int idx);
  void*	get_texture_by_flag(unsigned int flag);
  // Returns true if the given texture has the given flag, false otherwise.
  bool	texture_has_flag(void* texture, unsigned int flag);

# ifdef __cplusplus
}
# endif /* !__cplusplus */

#endif /* !ATEXTURESMANAGER_HPP_ */
