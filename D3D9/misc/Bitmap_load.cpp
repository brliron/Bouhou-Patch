#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<string>
#include	<stdio.h>

#include	"Bitmap.hpp"

#define	ABS(x)	((x) >= 0 ? (x) : (x) * -1)

void	Bitmap::XBGR_to_BGRA()
{
  char	red;
  char	green;
  char	blue;
  char	alpha;
  int	i;

  i = 0;
  while (i < this->width * this->height * 4)
    {
      alpha = 0xFF;
      blue = this->data[i + 1];
      green = this->data[i + 2];
      red = this->data[i + 3];

      this->data[i] = blue;
      this->data[i + 1] = green;
      this->data[i + 2] = red;
      this->data[i + 3] = alpha;

      i += 4;
    }
}

void	Bitmap::BGR_to_BGRA(const unsigned char* src)
{
  int	src_size;
  int	i;
  int	line_begin;
  int	j;

  if (this->width * 3 % 4 == 0)
    src_size = this->width * 3 * this->height;
  else
    src_size = (this->width * 3 + (this->width * 3 % 4)) * this->height;
  i = 0;
  line_begin = 0;
  j = 0;
  while (i < src_size)
    {
      this->data[j] = src[i];
      this->data[j + 1] = src[i + 1];
      this->data[j + 2] = src[i + 2];
      this->data[j + 3] = 0xFF;
      i += 3;
      if ((i - line_begin) / 3 == this->width)
	{
	  if ((i - line_begin) % 4 != 0)
	    i += 1 - ((i - line_begin) % 4);
	  line_begin = i;
	}
      j += 4;
    }
}

void				Bitmap::reverse_bitmap(int lineSize, int height)
{
  std::vector<unsigned char>	dest;
  int				y;

  dest.resize(this->data.size());
  y = 0;
  while (y < this->height)
    {
      std::copy(this->data.data() + (height - y - 1) * lineSize,
		this->data.data() + (height - y) * lineSize,
		dest.data() + y * lineSize);
      y++;
    }
  this->data = dest;
}

void	Bitmap::convert_bitmap(int compression)
{
  if (compression == BI_BITFIELDS)
    this->XBGR_to_BGRA();
  this->reverse_bitmap(4 * this->width, this->height);
}

bool		Bitmap::parse_header(const char* name, const BmpHeader& bmpHeader, const BmpInfoHeader& bmpInfo, int* offset, int* compression, int* bpp, int* size)
{
  if (bmpHeader.magic != BM_MAGIC)
    {
      wprintf(L"The file %S isn't a bitmap.\n", name);
      return false;
    }
  if (bmpInfo.bpp != 24 && bmpInfo.bpp != 32)
    {
      wprintf(L"The file %S must have 24 or 32 bits per pixels.\n", name);
      return false;
    }
  if (bpp)
    *bpp = bmpInfo.bpp;
  if (bmpInfo.compression != BI_RGB && bmpInfo.compression != BI_BITFIELDS)
    {
      wprintf(L"The compression of file %S isn't supported.\n", name);
      return false;
    }
  if (compression)
    *compression = bmpInfo.compression;

  this->width = bmpInfo.width;
  this->height = ABS(bmpInfo.height);
  if (offset)
    *offset = bmpHeader.offset;
  if (size)
    *size = bmpHeader.size - bmpHeader.offset;
  return true;
}

bool		Bitmap::load_bitmap(const char *name, int& compression)
{
  FILE*		file;
  BmpHeader	bmpHeader;
  BmpInfoHeader	bmpInfo;
  int		bpp;
  int		size;

  file = fopen(name, "rb");
  if (!file)
    {
      wprintf(L"Could not open file %S\n", name);
      return false;
    }

  fread(&bmpHeader, sizeof(BmpHeader), 1, file);
  fread(&bmpInfo, sizeof(BmpInfoHeader), 1, file);

  this->parse_header(name, bmpHeader, bmpInfo, NULL, &compression, &bpp, &size);
  if (bpp == 24)
    {
      wprintf(L"24 bpp images are supported only when loaded from memory.\n");
      return false;
    }

  this->data.resize(size);
  fseek(file, bmpHeader.offset, SEEK_SET);
  if (fread(this->data.data(), size, 1, file) <= 0)
    {
      perror("fread");
      wprintf(L"Could not read file %S\n", name);
      return false;
    }
  fclose(file);
  return true;
}

bool	Bitmap::load(const char* name)
{
  int	compression;

  if (this->load_bitmap(name, compression) == false)
    return false;
  this->convert_bitmap(compression);
  return true;
}

bool	Bitmap::load(const std::string& name)
{
  return this->load(name.c_str());
}

bool	Bitmap::load_from_memory(const unsigned char* data)
{
  int	offset;
  int	compression;
  int	bpp;
  int	size;

  this->parse_header("from memory",
		     *reinterpret_cast<const BmpHeader*>(data),
		     *reinterpret_cast<const BmpInfoHeader*>(data + sizeof(BmpHeader)),
		     &offset, &compression, &bpp, &size);

  this->data.resize(this->width * this->height * 4);
  if (bpp == 24)
    this->BGR_to_BGRA(data + offset);
  else
    memcpy(this->data.data(), data + offset, size);;
  this->convert_bitmap(compression);
  return true;
}
