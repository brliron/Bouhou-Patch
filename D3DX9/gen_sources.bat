@prompt -----^> 
@set LIBNAME=D3DX9_32

gcc gen_sources.c exports.c -Wall -Wextra -DLIB_NAME=\"%LIBNAME%\" -o gen_sources.exe
gen_sources
@del gen_sources.exe 2>nul

@prompt ^> 
