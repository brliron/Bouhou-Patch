# Bouhou Patch
A translation patch engine for Bouhou Koumakyou, Bouhou Youyoumu and Bouhou Koumakyou 2. And for New Super Marisa Land.\
It can also be expanded to support more games.

## How to install
- Go to the `Release` tab and download the last release.
- Unzip everything into the game directory
- Run the game

If you need more help open the README.txt in the release. What follows is a *technical* description of this project.

## How does it work
This project was designed when I didn't really know how to read assembly language. So, unlike a lot of translation patches, it was designed so that 99.9% of the work can be done without knowing assembly. And the 0.1% left can be auto-generated.\
This requirement led this patch to be built around detouring functions: wait until the game calls a function in a DLL, sneak in there, and change things just before they are displayed. To use a basic example, let's take the SetWindowText function. The game calls SetWindowTextA with a japanese window name. This patch can detour the SetWindowTextA function (which means the game calls the patch's SetWindowTextA instead of the user32.dll's SetWindowTextA), and call SetWindowTextW by passing a translated window name instead of passing the SetWindowText original parameter. The result is that the window text is translated.\
This is done for the configuration tool, in D3D9/game/BohoConfig/APatch.cpp.\
Detouring a Direct3D function isn't really hard. When the game calls Direct3DCreate9, it loads D3D9.dll (looking first in its directory and then in the system directories), looks for the Direct3DCreate9 function in it, and calls it. That's just how DLLs work (`kernel32.dll`, `user32.dll` and a few other DLLs are special and are only looked for in system directories, but `D3D9.dll` and `D3DX9_NN.dll` aren't special). So we just need to call our DLL `D3D9.dll`, and to make sure it has a function called Direct3DCreate9.

Functions detouring works for C functions, but most of Direct3D is built around C++ object and calls to member functions. On the other hand, since Direct3D objects are always accessed through an interface, we can detour its member functions quite easily; we just have to return our own object derived from the interface, and to implement all the virtual methods in the interface. For some reason, it doesn't work with IDirect3DTexture9, but it works with IDirect3D9 and IDirect3DDevice9, which gives us quite a lot of functions to detour.\
`implement all the virtual methods in the interface`: of course we don't want to reimplement everything, that would mean rewriting Direct3D9 ourselves. What we do instead is that our object have a copy of the real Direct3D object. When one of our method is called, we just call the same method on the real Direct3D object. Here is an example from D3D9/engine/MyD3D9.cpp:
```cpp
UINT	MyD3D9::GetAdapterCount()
{
  return orig->GetAdapterCount();
}
```

### Textures
Replacing textures is quite easy. You take a function call that deals with textures, and you replaces a parameter. For D3DXCreateTextureFromFileInMemoryEx, we just call D3DXCreateTextureFromFileExW with a different file (it has a bunch of stuff around it, to choose the right file, to save it somewhere if we don't know it (so that a translator can take care of it later), or for bookkeeping, but the core idea is that). For Direct3DDevice9::SetTexture, we just call the original SetTexture function with a different IDirect3DBaseTexture9 parameter.

### Texts
Texts are harder. Well, it would have been easy if the game just used something like TextOut, it would have been easy. But it doesn't work like that.\
The game uses a bitmap font: a huge image that contains an image of every character it needs. When it wants to display some text, it loops over every character in the text, looks for that character in the bitmap font, and displays that portion of the bitmap font. Our Direct3DTexture9::SetTexture detour have some logic to detect when the bitmap font is the current texture, and in DrawPrimitiveUP, when the bitmap font is selected, we look at the coordinates in the source texture and convert these coordinates into a character with a lookup table. We store this somewhere, because we can't translate it for now - if I just have 'A', I can't know if I need to print the translation for "Ananas", for "Apple", or for any other word.\
And we returns without calling the original DrawPrimitiveUP, because we may want to translate this text, we don't want to display the Japanese one!

Later, the game will call SetTexture again. If it wants to use another texture, it means it is done with the font texture, which means any "render this line" loop have to be done. All the character we stored until now have to make 1 or more full lines. So we will try different ways of pasting all these characters together, or split them by groups, and compare that to our translation database, in order to try to find the original texts and their translations. Yes, we might have more than one text on the screen. Look at the pause menu for example. A big PAUSE in the middle, and 4 entries below it. Also, we try to split them using their coordinates, because something in the HUD at the top obviously isn't in the same text block as something in the text box at the bottom, but this text box have 2 lines which obviously belong to the same text block. And a lot of times, we don't actually have an entire text block. The game uses a nice effect where it displays the text progressively, which means that at a given frame, the text may not be entirely displayed (like, if a sentance have 6 characters, we can imagine that only 2 characters are displayed in the 1st frame, 4 characters are displayed in the next frame, and it's only at the 3rd frame that the game displays all 6 characters). But we don't want to display the japanese text in the frames where we don't have the entire text.\
These are the things I think about as I write these lines, check the source code and you may find some other nice surprises.

So, we managed, through some miracle, to get a list of Japanese characters that were originally displayed as a text block by the game, and we found the translation of this text block in our lookup table. That was the hard part. Now, we just need to go through every character in our translation, get its position in the bitmap font, choose where we want to display it (using the position of the 1st original character, the width of the new character, and the size of the textbox given in a config file (to insert line breaks)), and display it by calling the original DrawPrimitiveUP.

### Drawbacks
The way this patcher works have 2 big drawbacks.
- It doesn't have the original data. Just look at the section just above, and go look at how many lines of code were written to convert a list of coordinates into a sentence. Moreover, in order to convert a pair of coordinates in a bitmap font into a character, I had to give someone an image of EVERY SINGLE CHARACTER used by the game, and for every image, he had to write the character in the image. If I just had the text in its original format, it would have been way easier.
- Texts and images are discovered when Direct3D sees them. And the game gives them to Direct3D only when it wants to display them on the screen. Which means you have to do a full playthrough of the game in Japanese, and to ensure that every single line of text have been displayed at least once, because if you don't, you won't know the lines that have never been displayed. And if you don't know those lines even exist, how would you translate them?

This is why I stopped working on this project and focused on the thcrap patching tool when I got enough experience with reverse engineering. But if you're interested in games patching but assembly is too hard for you, this might be a great project for you.

## How to build
When I look at the Makefiles, I think I was building and running it by using the MinGW compiler from the Windows cmd.exe. Since then, I switched to MinGW-w64 with the MSYS2 shell, and I may switch to WSL, but I didn't rewrite the Makefiles since I switched, so you'll have to deal with that old environment, or to rewrite the build files (I'll gladly accept a pull request with build files working on either MSYS2, WSL or Visual Studio).
- To build `D3D9`, go to the D3D9 directory and type `make`.
- To build `D3DX9`, run `build.bat`.
- To build `source_from_class`, run `gcc source_from_class.c -o source_from_class.exe`.
- The Tools directory contains a bunch of things. To compile `1_update_chars_list`, `2_update_dialogs` and `3_resplit_lines`, go in the Tools directory and type `make`. To compile all the other ones, go in their directories and type `gcc something.c` (with `something.c` being the source file in the directory). Same thing for `symlink.c` at the root.

# Components
## D3D9
D3D9 contains all the code that ends up in D3D9.dll. It contains the entire is the patch engine.

### engine
The part that actually patches Direct3D9.

### patch
Everything else.

### game
Game-specific code.

#### Boho/1
Code for Bouhou Koumakyou.

#### Boho/2
Code for Bouhou Youyoumu.

#### Boho/3
Code for Bouhou Koumakyou 2.

#### BohoConfig
Code for config.exe

#### D3DTextures
Code for generic texture patching. May be enough to patch games without a text engine, where all the text is in textures.\
Used for New Super Marisa Land.

#### None
Code for unknown games. Don't patch anything, just let the game running.

### misc
A bunch of small or bigger independent functions, that can be copy-pasted to other projects.

#### Archive
C++ wrapper around libarchive for tar files.

#### Bitmap
C++ class for reading, writing and mapulating bitmap files (copy-pasted from another project).

#### chcp
Small function to change the code page returned by GetACP to the desired value. Fixes a lot of problems when running a Japanese game on a non-Japanese locale.

#### Detour
A class to detour a function by binary patching. Used to detour functions from user32.dll.\
Example with SetWindowTextA:
```cpp
// The 1st template parameter is the return value, the others are the detoured function parameters.
// The Detour constructor doesn't take any parameter.
// The detoured function must use the stdcall calling convention, and it must have a return value.
Detour<BOOL, HWND, LPCSTR>*                      SetWindowTextADetour = new Detour<BOOL, HWND, LPCSTR>();
SetWindowTextADetour->init("User32.dll", "SetWindowTextA", MySetWindowTextA);
// ...
BOOL WINAPI	MySetWindowTextA(HWND hwnd, LPCSTR lpString)
{
  return SetWindowTextADetour->call_orig(hwnd, lpString);
}
```
There is no performance penality when executing a detoured function. On the other hand, the `call_orig` call enters a critical section during the whole call to the original function, and it calls FlushInstructionCache 2 times. And I just noticed a race condition: if a detoured function is called in the middle of a `call_orig` call, the detour won't work and the original function will be called.

#### DropdownBox
Like MessageBox, but with a dropdown menu instead of a few buttons.\
But it have been designed to run as a patch, without owning the host application, so there are a few odd things for a general-purpose dialog box.

#### FuncsCallStack
A stack to store pending function calls and execute them later. Used to offload slow operations to a background thread.\
It is used by `patch/BackgroundTasksManager.`*.\
Note: the size() function is broken.

#### md5
2 functions to compute the md5 of some data.

### `change_chars.cpp`
A small utility to change the properties of all the characters displayed by the patch. With it, you can, for example, adjust the width of the space character at runtime, fo find which space size gives the best look.\
For more details, look at the comment at the beginning of the file.

### libarchive
Just a static build of the libarchive.

## D3DX9
A wrapper around `D3DX9_XX.dll`, and some code to generate this wrapper.

### `gen_sources`
A tool to generate a wrapper around a DLL. To build it, edit the `@set LIBNAME=` line in `gen_sources.bat` to specify the DLL you want to wrap (without extension), then run `gen_sources.bat`. The original DLL must be accessible by LoadLibraryEx.\
It will create 2 files LIBNAME.asm and LIBNAME.def.

### LIBNAME
To compile your LIBNAME.asm and LIBNAME.def, you also need to edit the `@set LIBNAME=` line in build.bat, then to run build.bat. The generated DLL will forward every function call to the original DLL (which must be in System32 (or SysWoW64 for 32-bits processes on 64-bits OSes)).\
The overhead is 3 instructions per function call (and way more the 1st time every function is called).

After generating your wrapper, and before building it, you probably want to change a few functions. You can edit them in LIBNAME.asm (don't forget that running `gen_sources` will overwrite this file). If you need to write more complicated code, you can write your function in c in code.c, and add a call to your function in LIBNAME.asm (in the generated LIBNAME.asm, every function calls info FindEntry, you can use that as an example).

## `source_from_class`
Copy an interface definition into input.cpp, compile and run `source_from_class`, and it will write an implementation of this interface that forwards every member function call to the original object.\
You may need to tweak the input or the output in order to have something that gets parsed correctly and that can be compiled. Also, this tool doesn't write the constructor or the .hpp file for your class.

## Tools
A bunch of tools to parse the patch's output files.

As I wrote before, the patch will log the texts it doesn't know. But it might not get a lot of chances to log these texts - for example, the text might be displayed during the endings, so you have to finish the game to get it. And it might not be able to log the text - it may not know some characters, or it might split the text blocks wrong.\
So the logger won't try to be too clever, and it will spit as many things as possible. Which makes the log files huge (I've already seen a log file of 50 MB) and impossible to read. Huge files aren't that big of a problem, because they also have a huge compression ratio, but the "impossible to read" part means a human can't process them. But a computer can. And that's what these tools do.\
Each one of these tools is a step to make the log files more and more readable. They all have a number, which is the order in which you should run them. They take a file as input, and write another file as output. They all have a small readme. You should read it before using the tools, and you may also need to change a few things in the source files (for example, for the 0 one, you put the input directly in the source file).\
0 is about a legacy translations format, you shouldn't need it.\
1 tries to learn about characters it doesn't know. 2 uses this new knowledge to replace unknown characters in the log file with known characters. 3 uses this new knowledge of characters to try to do a better splitting.\
If you don't have any unkown character, you can skip 1->3 and run 3-1 instead.\
4 will remove the text blocks containing only a number, because you don't need to translate them. This one edits its files in place. It is a bit flaky, run it a few times until it removes everything. 5 will take care of the progressive display I talked about earlier, and keep only the fully displayed version of a text. And 6 will remove every text that you already knew, so that the result can be sent to a translator.\
And the symlink.c at the root can be used to create a symbolic link to the translations directory. I don't remember which tools need it, but I think some needs it.
