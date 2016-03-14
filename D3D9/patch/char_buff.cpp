#include	<stdio.h>
#include	"chars.hpp"
#include	"APatch.hpp"
#include	"Log.hpp"
#include	"ParseText.hpp"
#include	"Reader.hpp"


ACharBuff::ACharBuff()
  : flags(0), buff()
{
  Reader&	reader = Reader::get();

  if (reader.iniGetBool(L"CharBuff", L"DISABLE_BMP_SAVE"))
    this->flags |= CHARBUFF__DISABLE_BMP_SAVE;
  if (reader.iniGetBool(L"CharBuff", L"DISABLE_CHARS_LIST"))
    this->flags |= CHARBUFF__DISABLE_CHARS_LIST;
  if (reader.iniGetBool(L"CharBuff", L"DISABLE_LOGS"))
    this->flags |= CHARBUFF__DISABLE_LOGS;
  if (reader.iniGetBool(L"CharBuff", L"DISABLE_REPLACE_STR"))
    this->flags |= CHARBUFF__DISABLE_REPLACE_STR;
}

ACharBuff::ACharBuff(std::list<DisplayedChar*>& buff, int flags)
  : flags(flags), buff(buff)
{}

ACharBuff::~ACharBuff()
{}


bool	ACharBuff::empty() const
{
  return this->buff.empty();
}


void			ACharBuff::add_char(Char* c, float x1, float x2, float y1, float y2)
{
  DisplayedChar*	new_char;

  if (c == NULL)
    return ;
  if (c->c == L'\0')
    c->log(this->flags);
  new_char = new DisplayedChar;
  *new_char = *c;
  new_char->dst_x1 = x1 + 0.6;
  new_char->dst_x2 = x2 + 0.6;
  new_char->dst_y1 = y1 + 0.6;
  new_char->dst_y2 = y2 + 0.6;
  this->buff.push_back(new_char);
}


void	ACharBuff::put_str(const wchar_t* str, int x, int y)
{
  Char*	c;

  for (int i = 0; str[i] && str[i] != L'\n'; i++)
    {
      c = Char::get(str[i]);
      if (c)
	APatch::get().getTextDisplayer().display(*c, x, y);
      else
	my_log->put_text(L"A character is missing.\n");
      x += c->sw;
    }
}

int	DisplayedChar::get_abs_y() const
{
  if (this->sy != -1)
    return this->dst_y1 - this->sy;
  else
    return this->dst_y1;
}

bool	DisplayedChar::is_on_line(int line_y) const
{
  int	abs_y;

  abs_y = this->get_abs_y();
  return line_y - 5 <= abs_y && abs_y <= line_y + 5; // Note : I may accept a bigger margin on the top (20 px, for example).
}

void			replace_char(DisplayedChar* c)
{
  static HANDLE		hShm = NULL;
  static DisplayedChar*	shmem = NULL;
  int			x1;
  int			y1;

  if (hShm == NULL)
    {
      hShm = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(DisplayedChar), L"patch_char");
      shmem = (DisplayedChar*)MapViewOfFile(hShm, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DisplayedChar));
    }
  x1 = c->dst_x1;
  y1 = c->dst_y1;
  memcpy(c, shmem, sizeof(DisplayedChar));
  c->dst_x1 = x1;
  c->dst_y1 = y1 + c->sy;
  c->dst_x2 = x1 + c->sw;
  c->dst_y2 = c->dst_y1 + c->sh;
}

void			ACharBuff::dump_str()
{
  if (this->buff.empty())
    return ;

  ParseText		parseText(this->buff, this->flags);
  const DisplayedChar*	character;

  my_log->begin(this->buff);
  parseText.parse();
  while ((character = parseText.get_next_char()))
    {
      // if (/*this->buff.empty()*/character->c == L'A')
	// {
	  // replace_char(const_cast<DisplayedChar*>(character));
	  // const_cast<DisplayedChar*>(character)->c = L'A';
	// }
      APatch::get().getTextDisplayer().display(*character);
    }
  my_log->end();
}
