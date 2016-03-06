#ifndef BITMAP_HPP_
# define BITMAP_HPP_

# include	<fstream>
# include	<vector>
# include	<stdint.h>

# define	BM_MAGIC	0x4D42
# define	BI_RGB		0
# define	BI_BITFIELDS	3

#pragma pack(2)
struct		BmpHeader
{
  uint16_t	magic;
  uint32_t	size;
  uint16_t	_app1;
  uint16_t	_app2;
  uint32_t	offset;
};

#pragma pack(2)
struct		BmpInfoHeader
{
  uint32_t	size;
  int32_t	width;
  int32_t	height;
  uint16_t	planes;
  uint16_t	bpp;
  uint32_t	compression;
  uint32_t	raw_data_size;
  int32_t	h_resolution;
  int32_t	v_resolution;
  uint32_t	palette_size;
  uint32_t	important_colors;
};



/*
** A class encapsulating the BMP format. It can load a BMP file, change it, and save it on disk.
*/
class	Bitmap
{
private:
  std::vector<unsigned char>	data; // Note : I may think about storing this as an array of Color (where Color is a class which still doesn't exist).
  int				width;
  int				height;

  bool				load_bitmap(const char *name, int& compression);
  bool				parse_header(const char* name, const BmpHeader& bmpHeader, const BmpInfoHeader& bmpInfo, int* offset, int* compression, int* bpp, int* size);
  void				convert_bitmap(int compression);
  void				BGR_to_BGRA(const unsigned char* src);
  void				XBGR_to_BGRA();
  void				reverse_bitmap(int line_size, int height);

  bool				write_header(FILE* file, int compression);
  bool				write_data(FILE* file);
  bool				write_data_24bpp(FILE* file);
  void				create_padding(std::vector<unsigned char>& dest);
  void				BGRX_to_BGR(unsigned char* dest, unsigned char* src);
  void				BGRA_to_BGR(unsigned char* dest, unsigned char* src);

public:
  Bitmap();
  ~Bitmap();
  Bitmap&	operator=(const Bitmap& src);

  // Load a bitmap file.
  bool	load(const char *name);
  bool	load(const std::string& name);

  // Load a bitmap from the RAM.
  bool	load_from_memory(const unsigned char* data);

  // Shrink the bitmap. The specified area become the bitmap.
  void	shrink(int x, int y, int w, int h);

  // Save a 32 bpp bitmap file.
  bool	save(const char* name);

  // Save a 24 bpp bitmap file.
  bool	save_24bpp(FILE* name);
  bool	save_24bpp(const char* name);
  bool	save_24bpp(const wchar_t* name);
};

#endif /* !BITMAP_HPP_ */
