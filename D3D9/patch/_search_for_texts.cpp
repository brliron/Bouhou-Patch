/*
** Search for the given string in the whole process memory.
**
** The purpose of this file is to search for a displayed string in memory.
** Then, we search around this string, and we try to find other strings.
** Hopefully it would be able to find all the game's translatable content
** like this, by just seeing a single sentance in game.
**
** This file contains development code, not release code.
** These functions should not be called in a release for several reasons:
** - They are really slow (a few seconds). They scan the whole game memory
**   after all, that's quite a lot of data. Plus, with the way the boho
**   games display the strings progressively, these functions are called
**   several times for each string.
** - They seek for new texts to translate. In a release, we should translate
**   as much stuff as possible. Well, some part of the patch have been built
**   because we could have missed some translatable content, but this file
**   intend to not miss anything, so it shouldn't be needed in a release,
**   and hopefully it will make these other parts useless.
**   Yeah, I'm optimistic here...
** - I feel like this code have an average probability to be flagged by
**   an antivirus
**
** As this isn't release code, and as some things specific to a game may
** be useful for another game, I'd like anyone editing this file to not
** remove anything. If you don't need something, put it in a comment.
** If you developped something useful and you no longer need it after that,
** put it in a comment. If someone need to search for texts in another game,
** he might find the code you commented useful.
** Well, you can remove things if you're sure it'll never be useful for
** anything else. But think about it. I don't care having a lot of comments
** containing draft code in this file.
**
** To use it from another file, you should add the following prototype:
** void	search_for_texts(const wchar_t*);
** Calling this function will search for the given string.
** You can also add the following prototype instead:
** void	search_for_texts(std::list<DisplayedChar*>::iterator, std::list<DisplayedChar*>::iterator);
** It will search for a string made of the characters between the first and
** the second parameter.
** As sait previously, this is development code, and so I don't think about
** making a .hpp file for this function. Add the prototype in the cpp file
** when you need it, and remove it when you no longer need it.
**
** And finally, don't try to understand why some functions use snake_case
** and some other use camelCase. I don't know myself why I did that...
** I should refactor the whole project someday by choosing a naming
** convention, and stick with it.
*/

#include	<vector>
#include	<list>
#include	<map>
#include	<Windows.h>
#include	"chars.hpp"
#include	"Output.hpp"

static std::map<void*, DWORD>		memCache;
static std::vector<std::wstring>	seenStrings;

bool	isStringKnown(LPCWSTR str)
{
  static std::vector<std::wstring>	localSeenStrings;

  for (std::wstring& it : localSeenStrings)
    if (it == str)
      return true;
  localSeenStrings.push_back(str);
  return false;
}

bool		isValidSjis(unsigned char c)
{
  return c >= 0x20 && c != 0x7F;
}

void				buildMemCache()
{
  MEMORY_BASIC_INFORMATION	mbi;
  char*				addr;

  memCache.clear();
  addr = NULL;
  while (VirtualQuery(addr, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
      if ((mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_WRITECOPY)) && !(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT))
	{
	  // Output::printf(L"0x%.8x: size %d, alloc protection 0x%x, protection 0x%x\n", mbi.BaseAddress, mbi.RegionSize, mbi.AllocationProtect, mbi.Protect);
	  memCache[mbi.BaseAddress] = mbi.RegionSize;
	}
      addr = (char*)mbi.BaseAddress + mbi.RegionSize;
    }
}

bool	isValidPtr(const void* addr)
{
  MEMORY_BASIC_INFORMATION	mbi;
  if (VirtualQuery(addr, &mbi, sizeof(mbi)) != sizeof(mbi))
    return false;
  if ((mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_WRITECOPY)) && !(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT))
    return true;
  else
    return false;
}

std::vector<void*>	find_string(const char* str)
{
  std::vector<void*>	addresses;
  const unsigned int	str_len = strlen(str);

  for (std::pair<void*, DWORD> it : memCache)
    if (isValidPtr(it.first))
      for (char* addr = (char*)it.first; addr < (char*)it.first + it.second - str_len - 1; addr++) // TODO: do we really need -1 here?
	if (memcmp(addr, str, str_len) == 0)
	  addresses.push_back(addr);
  return addresses;
}

void		print_string(const char* ptr, int rec_dir)
{
  wchar_t*	wPtr;
  int		ret;

  for (int i = 0; ptr[i]; i++)
    if (isValidSjis(ptr[i]) == false)
      return ;
  SetLastError(0);
  ret = MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, ptr, -1, NULL, 0);
  if (ret == 0 || GetLastError() == ERROR_NO_UNICODE_TRANSLATION) // Yes, I use the OR operator, because MultiByteToWideChar doesn't seem to return 0 when it fails with ERROR_NO_UNICODE_TRANSLATION.
    return ;
  wPtr = (wchar_t*)malloc(ret * 2);
  MultiByteToWideChar(932, 0, ptr, -1, wPtr, ret);
  if (isStringKnown(wPtr) == false)
    Output::printf(L"Found %p%s: % s\n", ptr, ptr < (void*)0x00200000 ? L" (probably on the stack)" : L"", wPtr);

  // Searching for the previous string
  if (rec_dir <= 0)
    {
      const char*	prev_ptr = ptr - 1;
      // If the current string is preceded by 1 of a few \0, another string may precede.
      for (int i = 0; i < 4 && isValidPtr(prev_ptr) && *prev_ptr == '\0'; i++, prev_ptr--);
      if (prev_ptr != ptr - 1)
	{
	  // If we're in a string, we try to find its beginning.
	  while (isValidPtr(prev_ptr) && isValidSjis(*prev_ptr))
	    prev_ptr--;
	  prev_ptr++;
	  if (isValidPtr(prev_ptr) && isValidSjis(*prev_ptr))
	    print_string(prev_ptr, -1);
	}
    }

  // Searching for the next string
  if (rec_dir >= 0)
    {
      const char*	next_ptr = ptr + strlen(ptr) + 1;
      int		i;
      // Is there a string after this one? We skip a few \0 in order to check.
      for (i = 0; i < 4 && isValidPtr(next_ptr) && *next_ptr == '\0'; i++, next_ptr++);
      // We found something. Is this a string? We check all its characters.
      for (i = 0; isValidPtr(next_ptr + i) && isValidSjis(next_ptr[i]); i++);
      if (i != 0 && isValidPtr(next_ptr + i) && next_ptr[i] == '\0')
	print_string(next_ptr, 1);
    }
}

void	search_for_texts(const wchar_t* text)
{
  char*	cText;
  int	ret;

  static int i = 0;
  if (i++ < 4)
    return ;
  // Output::printf(L"Will look for %s...\n", text);
  for (std::wstring& it : seenStrings)
    if (it == text)
      {
	// Output::write(L"Already seen.\n");
	return ;
      }
  Output::printf(L"%s never seen. Will search it in memory...\n", text);
  seenStrings.push_back(text);

  ret = WideCharToMultiByte(932, 0, text, -1, NULL, 0, NULL, NULL);
  if (ret == 0)
    {
      Output::printf(L"WideCharToMultiByte of %s to shift-jis failed with code %d\n", text, GetLastError());
      return ;
    }
  cText = (char*)malloc(ret);
  WideCharToMultiByte(932, 0, text, -1, cText, ret, NULL, NULL);

  Output::write(L"Building memory cache...");
  buildMemCache();
  Output::write(L"done.\nCalling find_string...");
  std::vector<void*> strings = find_string(cText);
  Output::write(L"done.\n");
  for (void* it : strings)
    print_string((char*)it, 0);

  if (IsDebuggerPresent())
    asm("int3");
  free(cText);
}

void		search_for_texts(std::list<DisplayedChar*>::iterator it, std::list<DisplayedChar*>::iterator end)
{
  std::wstring	str;

  while (it != end)
    {
      str += (*it)->c;
      ++it;
    }
  search_for_texts(str.c_str());
}
