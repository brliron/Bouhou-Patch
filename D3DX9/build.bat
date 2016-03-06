@prompt -----^> 
@set LIBNAME=D3DX9_32

gcc -c code.c -Wall -Wextra -DLIB_NAME=\"%LIBNAME%\" "-DCUSTOM_ERR=\"Do you have DirectX on your computer ?\""
nasm %LIBNAME%.asm -f Win32
REM ld --kill-at -o %LIBNAME%.dll -shared code.o %LIBNAME%.obj %LIBNAME%.def "-LC:\Users\liron_b\Desktop\hacks\gen dll source\D3DX9\D3D9_" -lD3D9 -LE:\Program_Files\MinGW\lib -lKernel32 -lUser32 -e _DllMain
ld --kill-at -o %LIBNAME%.dll -shared code.o %LIBNAME%.obj %LIBNAME%.def "-LC:\Users\liron_b\Desktop\hacks\Boho patch\D3D9" -lD3D9 -LE:\Program_Files\MinGW\lib -lKernel32 -lUser32 -e _DllMain
@del code.o %LIBNAME%.obj 2>nul
@prompt ^> 
