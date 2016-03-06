#include	"Boho/CharBuff.hpp"
#include	"ATexturesManager.hpp"
#include	"Boho/TexturesFlags.hpp"
#include	"Boho/2/D3DDevice9.hpp"
#include	"Boho/3/D3DDevice9.hpp"


Boho::CharBuff::CharBuff(Boho::AD3DDevice9* d3dd)
  : ACharBuff(), d3dd(d3dd)
{}

Boho::CharBuff::CharBuff(std::list<DisplayedChar*>& buff, Boho::AD3DDevice9* d3dd, int flags)
  : ACharBuff(buff, flags), d3dd(d3dd)
{}

Boho::CharBuff::~CharBuff()
{}



static void	fill_vertex_tab(TLVERTEX tab[4], int x1, int x2, int y1, int y2, const Char& c)
{
  for (int i = 0; i < 4; i++)
    {
      tab[i].z = 0;
      tab[i].rhw = 1.0f;
      tab[i].colour = 0xFFFFFFFF;
    }

  tab[0].x = x1 - 0.5;
  tab[1].x = x2 - 0.5;
  tab[2].x = x1 - 0.5;
  tab[3].x = x2 - 0.5;
  tab[0].y = y1 - 0.5;
  tab[1].y = y1 - 0.5;
  tab[2].y = y2 - 0.5;
  tab[3].y = y2 - 0.5;
  tab[0].u = c.tx1;
  tab[1].u = c.tx2;
  tab[2].u = c.tx1;
  tab[3].u = c.tx2;
  tab[0].v = c.ty1;
  tab[1].v = c.ty1;
  tab[2].v = c.ty2;
  tab[3].v = c.ty2;
}

#ifndef DISABLE_DRAW_PRIMITIVE_UP

void	Boho::CharBuff::put_vertex_tab(TLVERTEX tab[4])
{
  this->d3dd->::D3DDevice9::DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, tab, 28);
}

void	Boho::CharBuff::put_char(const Char& c, int x1, int x2, int y1, int y2)
{
  TLVERTEX	tab[4];

  fill_vertex_tab(tab, x1, x2, y1, y2, c);
  this->put_vertex_tab(tab);
}

void	Boho::CharBuff::put_char(const Char& c, int x, int y)
{
  TLVERTEX	tab[4];

  fill_vertex_tab(tab, x, x + c.sw, y + c.sy, y + c.sy + c.sh, c);
  this->put_vertex_tab(tab);
}

void	Boho::CharBuff::put_char(const DisplayedChar& c)
{
  TLVERTEX	tab[4];

  fill_vertex_tab(tab, c.dst_x1, c.dst_x2, c.dst_y1, c.dst_y2, c);
  this->put_vertex_tab(tab);
}

#else
void	Boho::CharBuff::put_char(const Char&, int, int, int, int){}
void	Boho::CharBuff::put_char(const Char&, int, int){}
void	Boho::CharBuff::put_char(const DisplayedChar&){}
#endif /* !DISABLE_DRAW_PRIMITIVE_UP */



void	Boho::CharBuff::dump_str()
{
  IDirect3DBaseTexture9*	oldTexture;
  IDirect3DBaseTexture9*	fontTexture;
  IDirect3DPixelShader9*	oldPixelShader;

  if (this->buff.empty())
    return ;

  fontTexture = (IDirect3DBaseTexture9*)get_texture_by_flag(Boho::TexturesFlags::FONT);
  this->d3dd->orig->GetTexture(0, &oldTexture);
  if (oldTexture != fontTexture)
    this->d3dd->orig->SetTexture(0, fontTexture);

  if (Boho2::D3DDevice9::pixelShaderToUse != NULL) // TODO: remove boho2-specific and boho3-specific code
    {
      this->d3dd->orig->GetPixelShader(&oldPixelShader);
      this->d3dd->orig->SetPixelShader(Boho2::D3DDevice9::pixelShaderToUse);
    }
  if (Boho3::D3DDevice9::pixelShaderToUse != NULL)
    {
      this->d3dd->orig->GetPixelShader(&oldPixelShader);
      this->d3dd->orig->SetPixelShader(Boho3::D3DDevice9::pixelShaderToUse);
    }

  this->ACharBuff::dump_str();

  if (Boho2::D3DDevice9::pixelShaderToUse != NULL || Boho3::D3DDevice9::pixelShaderToUse != NULL)
    this->d3dd->orig->SetPixelShader(oldPixelShader);

  if (oldTexture != fontTexture)
    this->d3dd->orig->SetTexture(0, oldTexture);
  // if (!this->buff.empty())
    // this->buff.clear();
}
