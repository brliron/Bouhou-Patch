#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif
#include	<Windows.h>
#include	<string.h>
#include	"APatch.hpp"
#include	"None/Patch.hpp"
#ifdef ENABLE_BOHO
#include	"Boho/1/Patch.hpp"
#include	"Boho/2/Patch.hpp"
#include	"Boho/3/Patch.hpp"
#include	"BohoConfig/Boho1Patch.hpp"
#include	"BohoConfig/Boho2Patch.hpp"
#include	"D3DTextures/Patch.hpp"
#endif /* ENABLE_BOHO */
#include	"Reader.hpp"
#include	"Output.hpp"
#include	"Log.hpp"
#include	"BackgroundTasksManager.hpp"
#include	"ParseText.hpp"
#include	"md5.hpp"

APatch*	APatch::instance = NULL;

APatch::APatch()
  : texturesManager(NULL), charBuff(NULL)
{}

APatch::~APatch()
{
  delete this->texturesManager;
  this->texturesManager = NULL;
  delete this->charBuff;
  this->charBuff = NULL;
  APatch::instance = NULL;
}


ATexturesManager&	APatch::getTexturesManager()
{
  if (this->texturesManager == NULL)
    this->texturesManager = this->newTexturesManager();
  return *this->texturesManager;
}

ACharBuff&		APatch::getCharBuff()
{
  if (this->charBuff == NULL)
    this->charBuff = this->newCharBuff();
  return *this->charBuff;
}


void	APatch::MessageBox(LPCWSTR text, LPCWSTR title) const
{
  MessageBoxW(NULL, text, title, MB_OK);
}

HWND	APatch::getHwnd() const
{
  return this->hwnd;
}

/*
** On Windows 8 and later, the keymap is no longer handled per-windows, but globally.
** That means WM_INPUTLANGCHANGEREQUEST will change the keymap for the entire system
** on these OSes.
** TODO: either restore the old keymap each time the targetted window loses the focus,
** or hack direct input to translate the inputs.
*/
void	APatch::setHwnd(HWND hwnd)
{
  SetWindowTextW(hwnd, this->getWindowName());
  if (Reader::get().iniGetBool(L"global", L"qwerty"))
    SendMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, 0x409);
  this->hwnd = hwnd;
}


/*
** The APatch::get function should be called from the DllMain function.
** At this point, there is no guarantee the libc will be available.
** So we'll use our own wcscmp.
*/
int	my_wcscmp(const wchar_t *s1, const wchar_t *s2)
{
  while (*s1 && *s1 == *s2)
    {
      s1++;
      s2++;
    }
  return *s1 - *s2;
}

APatch&	APatch::get()
{
  if (APatch::instance != NULL)
    return *APatch::instance;

  WCHAR	exe_name[MAX_PATH];
  char	hash[33];
  WCHAR	wHash[33];
  WCHAR	game_name[128];

  GetModuleFileNameW(NULL, exe_name, MAX_PATH);
  md5(exe_name, hash);
  for (int i = 0; hash[i]; i++)
    wHash[i] = hash[i];
  wHash[32] = L'\0';
  GetPrivateProfileStringW(L"games", wHash, NULL, game_name, 128, L"translations\\games.ini");
  if (my_wcscmp(game_name, L"none") == 0)
    APatch::instance = new None::Patch();
#ifdef ENABLE_BOHO
  else if (my_wcscmp(game_name, L"boho1") == 0)
    APatch::instance = new Boho1::Patch();
  else if (my_wcscmp(game_name, L"boho1_config") == 0)
    APatch::instance = new BohoConfig::Boho1Patch();
  else if (my_wcscmp(game_name, L"boho2") == 0)
    APatch::instance = new Boho2::Patch();
  else if (my_wcscmp(game_name, L"boho2_config") == 0)
    APatch::instance = new BohoConfig::Boho2Patch();
  else if (my_wcscmp(game_name, L"boho3") == 0)
    APatch::instance = new Boho3::Patch();
  else if (my_wcscmp(game_name, L"D3DTextures") == 0)
    APatch::instance = new D3DTextures::Patch();
#endif /* ENABLE_BOHO */
  else
    {
      /*
      ** Yes, MessageBoxW isn't in Kernel32.dll, so it may no be loaded yet.
      ** But at this point, the patch won't work, so we can at least try to tell it to the user.
      ** And if it doesn't work, well... in a sense, the "The program stopped working" popup is
      ** a way to say to the user the patch didn't work...
      */
      MessageBoxW(NULL, L"Warning : this patch is running above an unknown game.\n"
			 "It will try to do as few things as possible, but it still may crash the game.", NULL, MB_OK);
      APatch::instance = new None::Patch();
    }
  return *APatch::instance;
}

bool	APatch::init()
{
  Reader&	reader = Reader::get();

  if (reader.init(this->getGameName()) == false)
    return false;
  reader.readFiles();
  reader.readCharsList();

  new Output();
  new EndManager();
  my_log = new Log();
  endManager->addObj(my_log);
  endManager->addObj(new BackgroundTasksManager());


  if (Reader::get().iniGetBool(L"SelfTest", L"enable", false) == true)
    this->selfTest();

  return true;
}

bool	APatch::hasInstance()
{
  return APatch::instance != NULL;
}



void	APatch::selfTest()
{
  FILE*	log;
  bool	result;

  log = fopen("selftest_log.txt", "w, ccs=UNICODE");
  if (log == NULL)
    this->MessageBoxF(NULL, L"Self test : could not open selftest_log.txt : %s", _wcserror(errno));
  Output::openConsole();
  Output::setFilePointer(log);

  Output::write(L"Self test begins...\n");
  {
    LPCWSTR	new_files_message_error = L"Some untranslated things were found while you was playing,\n"
					   "but the standard message to announce it "; // the 2 error messages begin like this.
    if (wcsncmp(Reader::get().getNewFilesMessage(), new_files_message_error, wcslen(new_files_message_error)) == 0)
      Output::printf(L"Error with new_files_message.txt. Displaying the error message...\n%s\n", Reader::get().getNewFilesMessage());
    else
      Output::write(L"new_files_message.txt OK.\n");
  }

  {
    result = true;
    Output::write(L"Beginning chars test...\n");
    LPCWSTR	en_text;
    for (int i = 0; ParseText::strings[i]; i += 2)
      {
	if (ParseText::strings[i][0] == L'#' || ParseText::strings[i + 1] == NULL)
	  continue ;
	en_text = ParseText::strings[i + 1];
	for (int j = 0; en_text[j]; j++)
	  if (Char::get(en_text[j]) == NULL)
	    {
	      Output::printf(L"Warning : unknown character %c in sentance %s\n", en_text[j], en_text);
	      result = false;
	    }
      }
    if (result == false)
      Output::write(L"The game will crash if it tries to display these translations.\n"
		     "Please add these character in chars_buff.txt.\n");
    Output::printf(L"Chars test %s.\n", result ? L"succeeded" : L"failed");
  }

  if (result)
    {
      result = true;
      Output::write(L"Beginning texts overflow test...\n");
      std::list<DisplayedChar*>	list;
      LPCWSTR			text;
      DisplayedChar		c;
      int			max_ret = INT_MAX; // If move_texts returns a value greater than this one, that means the text overflows.

      for (int i = 0; ParseText::strings[i]; i += 2)
	{
	  text = ParseText::strings[i];
	  if (text[0] == L'#')
	    {
	      if (wcscmp(text, L"#none") == 0)
		max_ret = INT_MAX;
	      else if (wcsncmp(text, L"#left", 5) == 0)
		{
		  LPWSTR tmp = wcsdup(text);
		  for (int i = 0; tmp[i]; i++)
		    if (tmp[i] == L';')
		      tmp[i] = L'_';
		  max_ret = _wtoi(Reader::get().iniGetString(L"SelfTest", tmp, L"2147483647" /* INT_MAX */));
		  free(tmp);
		}
	      else if (wcsncmp(text, L"#center", 7) == 0)
		max_ret = 0;
	      else
		continue ;
	    }
	  text = ParseText::strings[i + 1];
	  if (text == NULL)
	    continue ;
	  for (int j = 0; text[j]; j++)
	    {
	      c = *Char::get(text[j]);
	      c.sy = 0;
	      c.dst_x1 = 0;
	      list.insert(list.end(), new DisplayedChar(c));
	    }
	  if (move_texts(ParseText::strings, i, list.begin(), list.end()) > max_ret)
	    {
	      if (result == true)
		Output::write(L"The following line(s) overflow :\n");
	      result = false;
	      Output::write(text);
	      Output::write(L"\n");
	    }
	  for (DisplayedChar* it : list)
	    delete it;
	  list.clear();
	}
      Output::printf(L"Texts overflow test %s.\n", result ? L"succeeded" : L"failed");
    }
  else
    Output::write(L"The texts overflow test could not be run because the chars test failed.\n");

  Output::write(L"Self test finished.\n");
  Output::resetFilePointer();
  fclose(log);
}
