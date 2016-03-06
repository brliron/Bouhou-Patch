#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif /* __STRICT_ANSI__ */
#include	<Windows.h>
#include	<Wincrypt.h>
#include	<stdio.h>
#include	"md5.hpp"
#include	"md5_algo.hpp"

void		md5(LPCVOID data, UINT size, char str[33])
{
  MD5_CTX	ctx;
  BYTE		md5[16];
  int		i;

  MD5_Init(&ctx);
  MD5_Update(&ctx, data, size);
  MD5_Final(md5, &ctx);

  i = 0;
  while (i < 16)
    {
      sprintf(str + i * 2, "%.2hx", md5[i]);
      i++;
    }
}

void		md5(LPCWSTR filename, char str[33])
{
  HANDLE	hFile;
  BYTE*		data;
  UINT		size;
  DWORD		ret;

  hFile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == NULL)
    {
      /*
      ** Yes, swprintf, memset and MessageBoxW aren't in Kernel32.dll, so they may no be loaded yet.
      ** But at this point, the patch won't work, so we can at least try to tell it to the user.
      ** And if it doesn't work, well... in a sense, the "The program stopped working" popup is
      ** a way to say to the user the patch didn't work...
      */
      WCHAR	buff[MAX_PATH + 54];
      swprintf(buff, L"Error : could not open %s (Windows error %d).", filename, GetLastError());
      MessageBoxW(NULL, buff, NULL, MB_OK);
      memset(str, 0, 33);
      return ;
    }
  size = GetFileSize(hFile, NULL);
  data = new BYTE[size];
  ReadFile(hFile, data, size, &ret, NULL);
  CloseHandle(hFile);
  md5(data, size, str);
  delete[] data;
}
