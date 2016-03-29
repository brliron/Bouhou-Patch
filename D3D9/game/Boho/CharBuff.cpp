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
