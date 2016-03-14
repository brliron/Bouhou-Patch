#include	<stdio.h>
#include	<cmath>
#include	"Boho/D3DDevice9.hpp"
#include	"ATexturesManager.hpp"
#include	"Boho/TexturesFlags.hpp"
#include	"game/APatch.hpp"
#include	"chars.hpp"

void	Boho::AD3DDevice9::handlePixelShader()
{}



/*
** This function replaces each appearance of a given character with another character (it can for example replace each 3 with a 2).
** It is useless in the releases, but it was useful at the beginning, and it may be useful to begin new patches.
*/
/*
void	replace_char(TLVERTEX tab[4], Char* cur_char)
{
  static WCHAR	convert[] =
  {
    L'*', L'3',
    L'3', L'月',
    L'\0'
  };
  Char*	new_char;

  new_char = NULL;
  if (!cur_char)
    return ;
  for (int i = 0; convert[i] != L'\0'; i += 2)
    if (convert[i] == cur_char->c)
      {
	new_char = Char::get(convert[i + 1]);
	if (new_char == NULL)
	  return ;
	tab[0].u = new_char->tx1;
	tab[1].u = new_char->tx2;
	tab[2].u = new_char->tx1;
	tab[3].u = new_char->tx2;
	tab[0].v = new_char->ty1;
	tab[1].v = new_char->ty1;
	tab[2].v = new_char->ty2;
	tab[3].v = new_char->ty2;
	/ *
	** TO_DO : handle sw, sh and sy
	tab[1].x = tab[0].x + new_char->sw;
	tab[3].x = tab[0].x + new_char->sw;
	tab[2].y = tab[0].y + new_char->sh;
	tab[3].y = tab[0].y + new_char->sh;
	* /
	return ;
      }
}
*/

#include "Output.hpp"
HRESULT		Boho::AD3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  TLVERTEX*	tab;
  Char*		cur_char;

  if (texture_has_flag(this->curTexture, Boho::TexturesFlags::CHARACTERS_PICTURES))
    return this->printCharacterName(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
  tab = (TLVERTEX*)pVertexStreamZeroData;
  if (texture_has_flag(this->curTexture, Boho::TexturesFlags::FONT) == false ||
      std::isnan(tab[0].u))
    return orig->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
  cur_char = Char::get(tab[0].u, tab[0].v);
  // replace_char(tab, cur_char); // See the comment above to know why this line is commented

  this->handlePixelShader();

  if (cur_char)
    {
      if (cur_char->sw == 0)
	{
	  cur_char->sw = tab[1].x - tab[0].x;
	  cur_char->sh = tab[2].y - tab[0].y;
	}
      APatch::get().getCharBuff().add_char(cur_char, tab[0].x, tab[1].x, tab[0].y, tab[2].y);
      return D3D_OK;
    }
  else
    {
      Char tmp;
      tmp.c = L'\0';
      tmp.tx1 = tab[0].u;
      tmp.tx2 = tab[1].u;
      tmp.ty1 = tab[0].v;
      tmp.ty2 = tab[2].v;
      tmp.sy = -1;
      tmp.sw = tab[1].x - tab[0].x;
      tmp.sh = tab[2].y - tab[0].y;
      APatch::get().getCharBuff().add_char(&tmp, tab[0].x, tab[1].x, tab[0].y, tab[2].y);
      return D3D_OK;
    }
}
