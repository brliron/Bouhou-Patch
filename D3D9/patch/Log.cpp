#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#define	LOG_DEFINED
#include	<Shlwapi.h>
#include	<stdio.h>
#include	"Log.hpp"
#include	"EndManager.hpp"
#include	"Reader.hpp"

Log*	my_log = NULL;

Log::Log()
  : flags(0), fd(NULL), init(false)
{
  Reader&	reader = Reader::get();

  if (reader.iniGetBool(L"Log", L"HUMAN_READABLE_OUTPUT"))
    this->flags |= Log::HUMAN_READABLE_OUTPUT;
  if (reader.iniGetBool(L"Log", L"CREATE_NEW_FILE"))
    this->flags |= Log::CREATE_NEW_FILE;
  if (reader.iniGetBool(L"Log", L"DISABLE"))
    this->flags |= Log::DISABLE;
  if (reader.iniGetBool(L"Log", L"PRINT_ALL_CHARS"))
    this->flags |= Log::PRINT_ALL_CHARS;
  if (reader.iniGetBool(L"Log", L"PRINT_TRANSLATION"))
    this->flags |= Log::PRINT_TRANSLATION;
  if (reader.iniGetBool(L"Log", L"HIDE_KNOWN_TEXTS"))
    this->flags |= Log::HIDE_KNOWN_TEXTS;
}

Log::~Log()
{
  if (this->fd != NULL)
    {
      fclose(this->fd);
      this->fd = NULL;
    }
}


/*
** Open the log file
*/
void		Log::open_file()
{
  wchar_t	filename[MAX_PATH];

  if ((this->flags & Log::CREATE_NEW_FILE) == 0)
    {
      this->fd = _wfopen(L"dialogs.txt", L"w, ccs=UNICODE");
      endManager->addFile(L"dialogs.txt");
      return ;
    }
  if (PathIsDirectoryW(L"dialogs") == FALSE)
    CreateDirectoryW(L"dialogs", NULL);
  for (int i = 1; true; i++)
    {
      swprintf(filename, L"dialogs\\dialogs_%.3d.txt", i);
      if (endManager->isFileInArchive(filename) == false)
	{
	  this->fd = _wfopen(filename, L"w, ccs=UNICODE");
	  endManager->addFile(filename);
	  return ;
	}
    }
}

/*
** Open the file given as parameter, and use it as the log file
*/
bool	Log::open_file(LPCWSTR filename)
{
  if (this->flags & Log::DISABLE)
    return false;

  if (this->fd != NULL)
    fclose(this->fd);
  this->fd = _wfopen(filename, L"w, ccs=UNICODE");
  return this->fd != NULL;
}

/*
** Returns if we don't want to display anything (usually because the sentance is exactly like the previous one),
** then opens the correct file if needed.
*/
void	Log::begin(const std::list<DisplayedChar*>& buff)
{
  if (this->flags & Log::DISABLE)
    return ;

  this->init = false;
  this->charbuff_text = L"";
  for (const DisplayedChar* c : buff)
    this->charbuff_text += (c->c ? c->c : L'?');
  if (wcscmp(this->charbuff_text.c_str(), this->last_charbuff_text.c_str()) == 0)
    return ;

  this->init = true;
  this->text_written = false;
}

/*
** Prints the japanese line, and each character that should be displayed (depending on the settings).
** Also prints the translated text.
*/
void	Log::line(const std::list<DisplayedChar*>::iterator& begin, const std::list<DisplayedChar*>::iterator& end, LPCWSTR en_text)
{
  int	n;

  if (this->init == false || ((this->flags & Log::HIDE_KNOWN_TEXTS) && en_text != NULL))
    return ;

  if (this->fd == NULL)
    this->open_file();
  if (this->fd == NULL)
    return ;
  this->text_written = true;
  if (this->put_text_pending.empty() == false)
    {
      fwprintf(this->fd, L"%s", this->put_text_pending.c_str());
      this->put_text_pending.clear();
    }

  for (std::list<DisplayedChar*>::iterator it = begin ; it != end && (*it)->c != L'\n'; ++it)
    fputwc((*it)->c ? (*it)->c : L'?', this->fd);
  fputwc(L'\n', this->fd);
  n = 1;
  for (std::list<DisplayedChar*>::iterator it = begin ; it != end && (*it)->c != L'\n'; ++it)
    if ((*it)->c == L'\0' || (this->flags & Log::PRINT_ALL_CHARS))
      {
	const DisplayedChar*	c = *it;
	if (this->flags & Log::HUMAN_READABLE_OUTPUT)
	  fwprintf(this->fd, L"%dth character : %c    tx1=%f, tx2=%f, ty1=%f, ty2=%f, sy=%d, sw=%d, sh=%d, dst_x1=%d, dst_x2=%d, dst_y1=%d, dst_y2=%d\n", n, c->c ? c->c : L'?', c->tx1, c->tx2, c->ty1, c->ty2, c->sy, c->sw, c->sh, c->dst_x1, c->dst_x2, c->dst_y1, c->dst_y2);
	else
	  fwprintf(this->fd, L"%2d:%c %f %f %f %f %d %.2d %.2d  %.3d %.3d %.3d %.3d\n", n, c->c ? c->c : L'?', c->tx1, c->tx2, c->ty1, c->ty2, c->sy, c->sw, c->sh, c->dst_x1, c->dst_x2, c->dst_y1, c->dst_y2);
	n++;
      }
  if ((this->flags & Log::PRINT_TRANSLATION) && en_text)
    {
      fwprintf(this->fd, L"Translated text:");
      for (int i = 0; en_text[i] && en_text[i] != L'\n'; i++)
	fputwc(en_text[i], this->fd);
      fputwc(L'\n', this->fd);
    }
}

/*
** Logs a text with some \n.
*/
void	Log::line_with_nl(std::list<DisplayedChar*>::iterator it, const std::list<DisplayedChar*>::iterator& end, LPCWSTR en_text)
{
  while (it != end)
    {
      this->line(it, end, en_text);
      while (it != end && (*it)->c != L'\n')
	++it;
      ++it;
    }
}

/*
** Ends a call to begin()
*/
void	Log::end()
{
  if (this->fd == NULL || this->init == false)
    return ;
  if (this->text_written)
    fwprintf(this->fd, L"\n\n");
  this->last_charbuff_text = this->charbuff_text;
}

/*
** Puts an arbitrary string to the log file (usually beginning with #, to discern it from the others).
*/
void			Log::put_text(const wchar_t* str)
{
  if (this->flags & Log::DISABLE)
    return ;

  if (wcscmp(str, this->last_put_text.c_str()) != 0)
    {
      if (this->fd)
	fwprintf(this->fd, L"%s", str);
      else
	this->put_text_pending += str;
    }
  this->last_put_text = str;
}

void		log__put_text(const wchar_t* str)
{
  static bool	log_warning_triggered = false;

  if (my_log == NULL)
    {
      if (log_warning_triggered == false)
	MessageBoxW(NULL, L"Warning : log__put_text was called before creating a valid Log instance.\n"
			   "Please report this error message to brliron@hotmail.fr , and give as much details as you can (please give at lease the name of the game you're playing to).", NULL, MB_OK);
      log_warning_triggered = true;
      return ;
    }
  return my_log->put_text(str);
}
