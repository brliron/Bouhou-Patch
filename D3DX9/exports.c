#include	<Windows.h>
#include	<WinNT.h>
#include	<stdio.h>

HMODULE			load_module(LPCSTR name)
{
  HMODULE		hLib;
  PIMAGE_DOS_HEADER	dos_header;
  PIMAGE_NT_HEADERS	nt_header;

  hLib = LoadLibraryEx(name, NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (hLib == NULL)
    {
      fprintf(stderr, "LoadLibraryEx failed\n");
      return NULL;
    }

  dos_header = (PIMAGE_DOS_HEADER)hLib;
  if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
      fprintf(stderr, "DOS header: wrong magic number\n");
      return NULL;
    }

  nt_header = (PIMAGE_NT_HEADERS)((BYTE*)hLib + dos_header->e_lfanew);
  if (nt_header->Signature != IMAGE_NT_SIGNATURE)
    {
      fprintf(stderr, "NT header: wrong signature\n");
      return NULL;
    }

  return hLib;
}

int	print_exports(HMODULE hLib)
{
  BYTE*				lib;
  PIMAGE_DOS_HEADER		dos_header;
  PIMAGE_NT_HEADERS		header;
  PIMAGE_EXPORT_DIRECTORY	exports;
  DWORD*			names;

  lib = (BYTE*)hLib;
  dos_header = (PIMAGE_DOS_HEADER)lib;

  header = (PIMAGE_NT_HEADERS)(lib + dos_header->e_lfanew);
  if (header->OptionalHeader.NumberOfRvaAndSizes <= 0)
    {
      fprintf(stderr, "NT header error\n");
      return 1;
    }

  exports = (PIMAGE_EXPORT_DIRECTORY)(lib + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

  names = (DWORD*)(lib + exports->AddressOfNames);

  unsigned int	i;
  for (i = 0; i < exports->NumberOfNames; i++)
    printf("%s\n", lib + names[i]);

  return 0;
}

LPCSTR*				exports_to_tab(HMODULE hLib)
{
  BYTE*				lib;
  PIMAGE_DOS_HEADER		dos_header;
  PIMAGE_NT_HEADERS		header;
  PIMAGE_EXPORT_DIRECTORY	exports;
  DWORD*			names;
  LPCSTR*			res;

  lib = (BYTE*)hLib;
  dos_header = (PIMAGE_DOS_HEADER)lib;

  header = (PIMAGE_NT_HEADERS)(lib + dos_header->e_lfanew);
  if (header->OptionalHeader.NumberOfRvaAndSizes <= 0)
    {
      fprintf(stderr, "NT header error\n");
      return NULL;
    }

  exports = (PIMAGE_EXPORT_DIRECTORY)(lib + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

  names = (DWORD*)(lib + exports->AddressOfNames);

  res = malloc((exports->NumberOfNames + 1) * sizeof(LPCSTR));

  unsigned int	i;
  for (i = 0; i < exports->NumberOfNames; i++)
    res[i] = (LPCSTR)(lib + names[i]);
  res[i] = NULL;

  return res;
}

int	main_test()
{
  HMODULE	hLib;
  LPCSTR*	tab;

  hLib = load_module("kernel32.dll");
  if (hLib == NULL)
    return 1;
  tab = exports_to_tab(hLib);
  int i;
  for (i = 0; tab[i]; i++)
    printf("%s\n", tab[i]);
  return 0;
}
