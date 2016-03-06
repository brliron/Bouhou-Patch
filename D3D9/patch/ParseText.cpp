#include	"ParseText.hpp"
#include	"Log.hpp"
#include	"APatch.hpp"
#include	"Reader.hpp"

std::vector<LPCWSTR>	ParseText::strings;
int			ParseText::lastType = -1;

static int	wcsncmp_ignore_nl(const wchar_t* s1, const wchar_t* s2, int& length, bool keep_length = false)
{
  if (keep_length == false)
    length = 0;
  while (*s1 && *s1 == *s2)
    {
      s1++;
      s2++;
      length++;
    }
  if (*s1 == L'\0' || *s2 == L'\0')
    return 0;
  else if (*s1 == L'\n')
    return wcsncmp_ignore_nl(s1 + 1, s2, ++length, true); // Incrementing length may be a bad idea.
  else
    return *s1 - *s2;
}

static int	wcslen_stop_at_nl(const wchar_t* s)
{
  int		i;

  i = 0;
  while (s[i] && s[i] != L'\n')
    i++;
  return i;
}

void	go_to_new_line(LPCWSTR& s)
{
  s += wcslen_stop_at_nl(s);
  while (*s == L'\n')
    s++;
}

void	ParseText::replace(std::list<DisplayedChar*>::iterator& begin, int length, LPCWSTR new_str, int dest_max_length)
{
  std::list<DisplayedChar*>::iterator	end;
  DisplayedChar				c;
  int					x;
  int					y;
  int					i;

  end = begin;
  i = 0;
  while (i < length && end != this->buff.end())
    {
      ++end;
      i++;
    }
  x = (*begin)->dst_x1;
  y = (*begin)->get_abs_y();
  i = 0;
  while (new_str[i] && new_str[i] != L'\n' && dest_max_length > 0)
    {
      c = *Char::get(new_str[i]);
      c.dst_x1 = x;
      c.dst_x2 = x + c.sw;
      x += c.sw;
      c.dst_y1 = y + c.sy;
      c.dst_y2 = y + c.sy + c.sh;
      c.sy = 0;
      this->buff.insert(begin, new DisplayedChar(c));
      i++;
      dest_max_length--;
    }
  begin = this->buff.erase(begin, end);
}

void	ParseText::skip_line(std::list<DisplayedChar*>::iterator& it, LPCWSTR text, int dest_max_length)
{
  while (*text && dest_max_length > 0)
    {
      if (*text != L'\n')
        {
	  ++it;
	  dest_max_length--;
	}
      text++;
    }
}

int	ParseText::find_line(LPCWSTR str, int& ref_count)
{
  int	i;
  int	count; // Common characters between ParseText::strings[i] and str
  int	match_i; // i for the matching string
  int	match_count;

  match_i = -1;
  match_count = 0;
  for (i = 0; ParseText::strings[i]; i += 2)
    if (wcsncmp_ignore_nl(ParseText::strings[i], str, count) == 0)
      {
	if (count > match_count)
	  {
	    match_i = i;
	    match_count = count;
	  }
      }
  ref_count = match_count;
  return match_i;
}

////////////////////// PUT SOMEWHERE ELSE /////////////////////////////////
// void	search_for_texts(const wchar_t*);
////////////////////// PUT SOMEWHERE ELSE /////////////////////////////////
void	ParseText::replace_text(const std::list<DisplayedChar*>::iterator& begin)
{
  std::wstring	str;
  int		length;

  if (begin == this->buff.end())
    return ;

  for (std::list<DisplayedChar*>::iterator it = begin; it != this->buff.end(); ++it)
    if ((*it)->c == L'\0')
      {
	my_log->line(begin, this->buff.end(), NULL);
	return ;
      }
    else if ((*it)->c != L'\n')
      str += (*it)->c;

  int	i = find_line(str.c_str(), length);
  if (i == -1)
    {
      my_log->line(begin, this->buff.end(), NULL);
      return ;
    }

  const wchar_t*	jp_text = ParseText::strings[i];
  const wchar_t*	en_text = ParseText::strings[i + 1];
  std::list<DisplayedChar*>::iterator it = begin;

  if (en_text == NULL)
    {
      ParseText::skip_line(it, jp_text, length);
      return this->replace_text(it);
    }

  ////////////////////// PUT SOMEWHERE ELSE /////////////////////////////////
  // search_for_texts(jp_text);
  ////////////////////// PUT SOMEWHERE ELSE /////////////////////////////////

  while (*jp_text && it != this->buff.end())
    {
      my_log->line(it, this->buff.end(), en_text);
      std::list<DisplayedChar*>::iterator before_it = it;
      --before_it; // The node pointed by it may be removed during ParseText::replace. That's why we point to the previous node, then go back to the 1st node of the new string.
      this->replace(it, wcslen_stop_at_nl(jp_text), en_text, (float)wcslen(en_text) * ((float)length / (float)wcslen(jp_text)));
      ++before_it;
      move_texts(ParseText::strings, i, before_it, it);
      go_to_new_line(jp_text);
      go_to_new_line(en_text);
    }
  return this->replace_text(it);
}

void			ParseText::replace_line(std::list<DisplayedChar*>::iterator& begin)
{
  static const LPCWSTR	strings[] =
    {
/*
** French translations, for testing purposes.
** This function hasn't been tested for some months, and so it probably isn't up to date.
*/
      L"CAKE_S",	L"GATEAU S",
      L"CAKE_L",	L"GATEAU_L",
      // L"RED-SCRAP",	L"DECHET ROUGE",
      // L"BLUE-SCARP",	L"DECHET BLEU", // Note : the game has a typo here.
      // L"KAWARIMI",	L"",
      L"GLOVE",		L"GANT",
      // L"GRIMOIRE",	L"",
      L"BOOTS",		L"BOTTES",
      L"WING",		L"AILE",
      // L"BOOSTER",	L"",
      L"WRISTBAND",	L"BRACELET",
      L"RIBBON",	L"RUBAN",
      L"KNIFE",		L"COUTEAU",
      L"SPEAR",		L"LANCE",
      L"SWORD",		L"EPEE",
      L"POWER-RING",	L"ANNEAU DE PUISSANCE",
      L"RANGE-RING",	L"ANNEAU DE PORTEE",
      L"VITAL-RING",	L"ANNEAU DE VITALITE",
      L"SPEED-RING",	L"ANNEAU DE VITESSE",
      // L"YUMEGA-HIROGARING",		L"",
      L"SLOT",		L"EMPLACEMENT",
      L"KEY",		L"CLE",
      // L"YUKKURI_L",	L"",
      // L"YUKKURI_R",	L"",
      L"レミリア",		L"REMILIA",
      L"パチュリー",	L"PATCHOULI",
      L"咲夜",		L"SAKUYA",
      L"美鈴", 		L"MEILING",
      L"フランドール",	L"FLANDRE",
      // L"本の切れ端の様な赤い紙切れ。瞬く間にお家ヘ帰\nリます。", L"SMALL SCRAPS OF RED PAPER。 IN THE BLINK\nOF AN EYE THEY CAN RETURN YOU HOME。",
      L"本の切れ端の様な赤い紙切れ。瞬く間にお家ヘ帰", L"SMALL SCRAPS OF RED PAPER。 IN THE BLINK",
      L"リます。",		L"OF AN EYE THEY CAN RETURN YOU HOME。",
      NULL
    };
  std::wstring	str;

  for (std::list<DisplayedChar*>::iterator it = begin; (*it)->c != L'\n'; ++it)
    str += (*it)->c;
  for (int i = 0; strings[i]; i += 2)
    if (wcsncmp(str.c_str(), strings[i], wcslen(strings[i])) == 0)
      {
	my_log->line_with_nl(begin, this->buff.end(), strings[i + 1]);
	this->replace(begin, wcslen(strings[i]), strings[i + 1]);
	return ; // true
      }
  my_log->line_with_nl(begin, this->buff.end(), NULL);
  return ; // false
}

void	ParseText::replace_lines()
{
  for (std::list<DisplayedChar*>::iterator it = this->buff.begin(); it != this->buff.end(); ++it)
    {
      this->replace_line(it);
      while ((*it)->c != L'\n')
        ++it;
    }
}

ParseText::ParseText(std::list<DisplayedChar*>& buff, int flags)
  : buff(buff), flags(flags)
{
  if (ParseText::lastType != -1)
    {
      this->type = lastType;
      return ;
    }

  LPCWSTR	str;

  str = Reader::get().iniGetString(L"ParseText", L"mode", L"TRANSLATE_FIRST");
  if (wcscmp(str, L"TRANSLATE_FIRST") == 0)
    this->type = ParseText::TRANSLATE_FIRST;
  else if (wcscmp(str, L"SPLIT_FIRST") == 0)
    this->type = ParseText::SPLIT_FIRST;
  else
    {
      Reader::get().dispMessage(L"ERR_PARSETEXT_WRONG_MODE");
      this->type = ParseText::TRANSLATE_FIRST;
    }
  ParseText::lastType = this->type;
}

ParseText::~ParseText()
{}

void	ParseText::split_lines()
{
  std::list<DisplayedChar*>::iterator	it;
  DisplayedChar				end_line;
  int					y;

  end_line.c = L'\n';
  it = this->buff.begin();
  while (it != this->buff.end())
    {
      y = (*it)->get_abs_y();
      while (it != this->buff.end() && (*it)->is_on_line(y))
        {
	  if (y > (*it)->get_abs_y())
	    y = (*it)->get_abs_y();
	  ++it;
	}
      this->buff.insert(it, new DisplayedChar(end_line));
    }
}

void					ParseText::calc_y_shifts_line(std::list<DisplayedChar*>::iterator& it_begin)
{
  std::list<DisplayedChar*>::iterator	it;
  int					baseline;

  // Calculating baseline
  baseline = INT_MAX; // Don't use screen height, because the game can display some text outside the screen (example : Boho Koumakyou 1, ending)
  for (it = it_begin; (*it)->c != L'\n'; ++it)
    if ((*it)->c != L'\0')
      {
	if ((*it)->dst_y1 < baseline)
	  baseline = (*it)->get_abs_y();
      }

  // For each character, calculating y shift if it is unknown or incorrect.
  for (it = it_begin; (*it)->c != L'\n'; ++it)
    if ((*it)->c != L'\0')
      {
	Char*	c;

	c = Char::get((*it)->c);
	if (c == NULL || c->sy >= (*it)->dst_y1 - baseline)
	  continue ;
	c->sy = (*it)->dst_y1 - baseline;
	(*it)->sy = 0;
	(*it)->dst_y2 = baseline + c->sy + ((*it)->dst_y2 - (*it)->dst_y1);
	(*it)->dst_y1 = baseline + c->sy;
	(*it)->log(this->flags);
      }
    else
      (*it)->log(this->flags);
  
  ++it;
  it_begin = it;
}

void					ParseText::calc_all_y_shifts()
{
  std::list<DisplayedChar*>::iterator	it;

  it = this->buff.begin();
  while (it != this->buff.end())
    this->calc_y_shifts_line(it);
}

void	ParseText::parse()
{
  if (this->type == ParseText::SPLIT_FIRST)
    {
      this->split_lines();
      this->calc_all_y_shifts();
      this->replace_lines();
    }
  else if (this->type == ParseText::TRANSLATE_FIRST)
    {
      this->replace_text(this->buff.begin());
    }
  this->iterator = this->buff.begin();
}

const DisplayedChar*	ParseText::get_next_char()
{
  const DisplayedChar*	character;

  if (this->iterator != this->buff.end() && (*this->iterator)->c == L'\n')
    ++this->iterator;
  if (this->iterator == this->buff.end())
    {
      for (DisplayedChar* it : this->buff)
	delete it;
      this->buff.clear();
      return NULL;
    }
  character = *this->iterator;
  ++this->iterator;
  return character;
}
