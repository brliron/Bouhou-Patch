#include	<Windows.h>
#include	"Output.hpp"

static std::map<void*, DWORD>		memCache;
static std::vector<std::wstring>	seenStrings;

void				buildMemCache()
{
  MEMORY_BASIC_INFORMATION	mbi;
  char*				addr;

  memCache.clear();
  addr = NULL;
  while (VirtualQuery(addr, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
      if (mbi.AllocationProtect & (PAGE_READONLY | PAGE_EXECUTE_WRITECOPY | PAGE_READWRITE | PAGE_WRITECOMBINE) && mbi.State & MEM_COMMIT)
	memCache[mbi.BaseAddress] = mbi.RegionSize;
      addr = mbi.BaseAddress + mbi.RegionSize;
    }
}

std::vector<void*>	findString(const char* str)
{
  std::vector<void*>	addresses;
  const unsigned int	str_len = strlen(str);

  for (std::pair<void*, DWORD> it : memCache)
    for (char* addr = it.first; addr < (char*)it.first + it.second - str_len - 1; addr++) // TODO: do we really need -1 here?
      if (memcmp(addr, str, str_len) == 0)
	addresses.push_back(addr);
  return addresses;
}

void	search_for_texts(const wchar_t* text)
{
  char*	cText;
  int	ret;

  Output::printf("Will look for %s...\n", text):
  for (std::wstring& it : seenStrings)
    if (it == text)
      {
	Output::write("Already seen.\n");
	return ;
      }
  Output::write("Never seen. Will search it in memory...\n"):
  seenStrings.push_back(text);

  ret = WideCharToMultiByte(932, 0, text, -1, NULL, 0, NULL, NULL);
  if (ret == 0)
    {
      Output::printf(L"WideCharToMultiByte of %s to shift-jis failed with code %d\n", text, GetLastError());
      return ;
    }
  cText = malloc(ret);
  WideCharToMultiByte(932, 0, text, -1, cText, ret, NULL, NULL);

  buildMemCache();
  std::vector<void*> strings = findString(cText);
  for (void* it : string)
    {
      char*	ptr = (char*)it;
      char*	guessedBeginning;

      Output::printf(L"Found %p%s\n", ptr, ptr < 0x00200000 ? L" (probably on the stack)" : L"");
      Output::write(L"Trying to guess earlier beginning...\n");
      guessedBeginning = ptr;
      while (isValidPtr() && isValidSjis(ptr))
	ptr--;
      ptr++;
    }

  free(cText);
}
