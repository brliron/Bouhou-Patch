#include	<stdexcept>
#include	"Boho/Apatch.hpp"
#include	"Boho/CharBuff.hpp"
#include	"Boho/TexturesManager.hpp"
#include	"DrawPrimitiveUpDisplayer.hpp"
#include	"chcp.hpp"

Boho::APatch::APatch()
{
  chcp(932);
}

ACharBuff*	Boho::APatch::newCharBuff(int) const
{
  throw std::logic_error("The Boho::CharBuff class must be created with a Direct3D device.");
}

ACharBuff*	Boho::APatch::newCharBuff(Boho::AD3DDevice9* d3dd) const
{
  return new Boho::CharBuff(d3dd);
}

ATexturesManager*	Boho::APatch::newTexturesManager() const
{
  return new Boho::TexturesManager();
}

ATextDisplayer*	Boho::APatch::newTextDisplayer() const
{
  throw std::logic_error("The TextDisplayer for the Bouhou games must be created with a Direct3D device.");
}

ATextDisplayer*	Boho::APatch::newTextDisplayer(::D3DDevice9* d3dd) const
{
  return new DrawPrimitiveUpDisplayer(d3dd);
}

ACharBuff*	Boho::APatch::storeNewCharBuff(Boho::AD3DDevice9* d3dd)
{
  if (this->charBuff == NULL)
    this->charBuff = this->newCharBuff(d3dd);
  return this->charBuff;
}


bool	Boho::APatch::isGameExiting(CWPSTRUCT* msgParams) const
{
  char	buff[256];

  if (GetWindowText(msgParams->hwnd, buff, 255) <= 0)
    return false;
  if (strcmp(buff, "CicMarshalWnd") == 0)
    return true;
  return false;
}

void	Boho::APatch::initD3D(HWND hFocusWindow, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  this->AD3DPatch::initD3D(hFocusWindow, pPresentationParameters);
  this->charBuff = this->newCharBuff(dynamic_cast<Boho::AD3DDevice9*>(this->D3DDevice));
  this->textDisplayer = this->newTextDisplayer(this->D3DDevice);
}
