#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<string>
#include	<stdio.h>

#include	"Bitmap.hpp"

Bitmap::Bitmap()
{}

Bitmap::~Bitmap()
{}

Bitmap&	Bitmap::operator=(const Bitmap& src)
{
  this->data = src.data;
  this->width = src.width;
  this->height = src.height;
  return *this;
}

int	Bitmap::getWidth() const
{
  return this->width;
}

int	Bitmap::getHeight() const
{
  return this->height;
}

void				Bitmap::shrink(int x, int y, int w, int h)
{
  std::vector<unsigned char>	vector_dest;
  int*				src;
  int*				dest;

  vector_dest.resize(w * h * 4);
  src = reinterpret_cast<int*>(this->data.data());
  dest = reinterpret_cast<int*>(vector_dest.data());
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      dest[j * w + i] = src[(y + j) * this->width + (x + i)];
  this->data = vector_dest;
  this->width = w;
  this->height = h;
}



bool		Bitmap::write_header(FILE* file, int compression)
{
  BmpHeader	bmpHeader;
  BmpInfoHeader	bmpInfo;

  bmpHeader.magic = BM_MAGIC;
  bmpHeader.offset = sizeof(BmpHeader) + sizeof(BmpInfoHeader);
  bmpHeader._app1 = 0;
  bmpHeader._app2 = 0;

  bmpInfo.size = sizeof(BmpInfoHeader);
  bmpInfo.width = this->width;
  bmpInfo.height = this->height;
  bmpInfo.planes = 1;
  bmpInfo.bpp = (compression == BI_BITFIELDS ? 32 : 24);
  bmpInfo.compression = compression;
  if (compression == BI_BITFIELDS)
    bmpInfo.raw_data_size = this->width * this->height * 4;
  else
    {
      int	width = this->width * 3;
      if (width % 4 != 0)
	width += 4 - (width % 4);
      bmpInfo.raw_data_size = width * this->height;
    }
  bmpInfo.h_resolution = 65535;
  bmpInfo.v_resolution = 65535;
  bmpInfo.palette_size = 0;
  bmpInfo.important_colors = 0;

  bmpHeader.size = bmpHeader.offset + bmpInfo.raw_data_size;

  fwrite(&bmpHeader, sizeof(BmpHeader), 1, file);
  fwrite(&bmpInfo, sizeof(BmpInfoHeader), 1, file);
  return true;
}

bool	Bitmap::write_data(FILE* file)
{
  this->reverse_bitmap(4 * this->width, this->height);
  fwrite(this->data.data(), 4 * this->width * this->height, 1, file);
  return true;
}

bool		Bitmap::save(const char* name)
{
  FILE*		file;

  file = fopen(name, "wb");
  if (!file)
    {
      wprintf(L"Could not open file %S\n", name);
      return false;
    }
  this->write_header(file, BI_BITFIELDS);
  this->write_data(file);
  return true;
}

void	Bitmap::BGRX_to_BGR(unsigned char* dest, unsigned char* src)
{
  dest[0] = src[0];
  dest[0] = src[1];
  dest[0] = src[2];
}

void	Bitmap::BGRA_to_BGR(unsigned char* dest, unsigned char* src)
{
  float	src_a;

  src_a = src[3] / 255.0;
  dest[0] = src[0] * src_a + 0xFF * (1 - src_a);
  dest[1] = src[1] * src_a + 0xFF * (1 - src_a);
  dest[2] = src[2] * src_a + 0xFF * (1 - src_a);
}

void	Bitmap::create_padding(std::vector<unsigned char>& dest)
{
  int	w;

  w = this->width * 3;
  if (w % 4 != 0)
    w += 4 - (w % 4);
  dest.resize(w * this->height);

  for (int j = 0; j < this->height; j++)
    {
      int	i;
      for (i = 0; i < this->width; i++)
	this->BGRA_to_BGR(dest.data() + (j * w + i * 3), this->data.data() + (j * (this->width * 4) + i * 4));
      i *= 3;
      while (i % 4 != 0)
	{
	  dest[j * w + i] = 0;
	  i++;
	}
    }
}

bool	Bitmap::write_data_24bpp(FILE* file)
{
  std::vector<unsigned char>	dest;
  int				w;

  this->reverse_bitmap(4 * this->width, this->height);
  this->create_padding(dest);
  w = this->width * 3;
  if (w % 4 != 0)
    w += 4 - (w % 4);
  fwrite(dest.data(), w * this->height, 1, file);
  return true;
}

bool		Bitmap::save_24bpp(FILE* fd)
{
  this->write_header(fd, BI_RGB);
  this->write_data_24bpp(fd);
  fclose(fd);
  return true;
}

bool		Bitmap::save_24bpp(const char* name)
{
  FILE*		fd;

  fd = fopen(name, "wb");
  if (fd == NULL)
    {
      wprintf(L"Could not open file %S\n", name);
      return false;
    }
  return this->save_24bpp(fd);
}

bool		Bitmap::save_24bpp(const wchar_t* name)
{
  FILE*		fd;

  fd = _wfopen(name, L"wb");
  if (fd == NULL)
    {
      wprintf(L"Could not open file %s\n", name);
      return false;
    }
  return this->save_24bpp(fd);
}

/*Color		Bitmap::operator[](int idx) const
{
  unsigned char	r;
  unsigned char	g;
  unsigned char	b;
  unsigned char	a;

  b = this->_data[idx * 4];
  g = this->_data[idx * 4 + 1];
  r = this->_data[idx * 4 + 2];
  a = this->_data[idx * 4 + 3];

  return (Color(r, g, b, a, Color::RGB));
}*/
