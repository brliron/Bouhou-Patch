#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<stdio.h>
#include	<Shlwapi.h>
#include	"Boho/2/D3DDevice9.hpp"
#include	"Log.hpp"
#include	"Bitmap.hpp"
#include	"Output.hpp"
#include	"ATexturesManager.hpp"
#include	"Boho/TexturesFlags.hpp"

IDirect3DPixelShader9*	Boho2::D3DDevice9::pixelShaderToUse = NULL;

static void	write_bmp_to_file(const Bitmap* bmp_, float tx1, float ty1, float tx2, float ty2, LPWSTR filename)
{
  Bitmap	bmp;

  bmp = *bmp_;
  bmp.shrink((int)(tx1 * 1024) % 1024, (int)(ty1 * 1024) % 1024, (int)((tx2 - tx1) * 1024) % 1024, (int)((ty2 - ty1) * 1024) % 1024);
  if (bmp.save_24bpp(filename) == false)
    Output::printf(L"Warning : couldn't save face bitmap as %s.\n", filename);
}

static void _store(LPCWSTR name, float tx1, float ty1, float tx2, float ty2)
{
  static Bitmap*	bmp = NULL;
  WCHAR			out_filename[MAX_PATH];

  wsprintfW(out_filename, L"faces\\%s_%d_%d.bmp", name, (int)(tx1 * 10000), (int)(ty1 * 10000));
  if (PathFileExistsW(out_filename))
    return ;
  if (!PathIsDirectoryW(L"faces"))
    CreateDirectoryW(L"faces", NULL);
  if (bmp == NULL)
    {
      LPD3DXBUFFER	raw_bmp;

      bmp = new Bitmap;
      if (D3DXSaveTextureToFileInMemory(&raw_bmp, D3DXIFF_BMP, (LPDIRECT3DTEXTURE9)get_texture_by_flag(Boho::TexturesFlags::CHARACTERS_PICTURES), NULL) != D3D_OK ||
	  bmp->load_from_memory(static_cast<unsigned char*>(raw_bmp->GetBufferPointer())) == false)
	{
	  Output::printf(L"Warning : couldn't load the faces bitmap.\n");
	  delete bmp;
	  bmp = NULL;
	  return ;
	}
    }
  write_bmp_to_file(bmp, tx1, ty1, tx2, ty2, out_filename);
}

HRESULT		Boho2::D3DDevice9::printCharacterName(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  TLVERTEX*	tab;
  WCHAR		str[2048];
  const WCHAR*	name;

  tab = (TLVERTEX*) pVertexStreamZeroData;
  // if (float_cmp(tab[0].v, 0.882813) && float_cmp(tab[0].u, 0.062500) == false)
  if (tab[0].u < 0.4 && tab[0].v > 0.1251 && tab[0].v < 0.843750)
    {
      if (float_cmp(tab[0].v, 0.1562))
	{
	  if (tab[0].u < 0.01)
	    name = L"Letty";
	  else if (tab[0].u < 0.1)
	    name = L"Alice";
	  else if (tab[0].u < 0.2)
	    name = L"Lunasa";
	  else if (tab[0].u < 0.3)
	    name = L"Merlin";
	  else
	    name = L"Lyrica";
	}
      else if (float_cmp(tab[0].v, 0.2646) || (float_cmp(tab[0].v, 0.3730) && tab[0].u < 0.01))
	name = L"Youmu";
      else if (float_cmp(tab[0].v, 0.3730))
	name = L"Yuyuko";
      else if (float_cmp(tab[0].v, 0.4814) && tab[0].u < 0.2)
	name = L"Ran";
      else if (float_cmp(tab[0].v, 0.4814) || (float_cmp(tab[0].v, 0.5898) && tab[0].u < 0.2))
	name = L"Yukari";
      else if (float_cmp(tab[0].v, 0.5898))
	name = L"Reimu";
      else if (/* float_cmp(tab[0].v, 0.) && */ tab[0].u < 0.01)
	name = L"Cirno";
      else if (/* float_cmp(tab[0].v, 0.) && */ tab[0].u < 0.1)
	name = L"Sakuya";
      else
	name = L"An unknown character"; // or more likely a bug
      if (wcscmp(name, L"An unknown character") != 0)
	swprintf(str, L"## %s :\n", name);
      else
	swprintf(str, L"## %s is talking (u=%f, v=%f)\n", name, tab[0].u, tab[0].v);
      my_log->put_text(str);
      _store(name, tab[0].u, tab[0].v, tab[1].u, tab[2].v);
    }
  return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT	Boho2::D3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  this->curPixelShader = pShader;
  return orig->SetPixelShader(pShader);
}

void	Boho2::D3DDevice9::handlePixelShader()
{
  Boho2::D3DDevice9::pixelShaderToUse = curPixelShader;
}

