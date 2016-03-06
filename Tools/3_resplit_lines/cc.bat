@set D3DX9_DIR=..
@set D3D9_DIR=..\D3D9

g++ -c %D3D9_DIR%\game_specific\BohoCharBuff.cpp -o %D3D9_DIR%\game_specific\BohoCharBuff.o -DDISABLE_DRAW_PRIMITIVE_UP -DDISABLE_END_MANAGER -Wall -Wextra -Wno-comment -Wno-unknown-pragmas --std=c++11 -g -IE:\Program_Files\DX_SDK\Include -I%D3D9_DIR%\inject -I%D3D9_DIR%\replace -I%D3D9_DIR%\game_specific -I%D3D9_DIR%\misc -I%D3D9_DIR%\libarchive
g++ -c %D3D9_DIR%\replace\Char.cpp -o %D3D9_DIR%\replace\Char.o -DDISABLE_DRAW_PRIMITIVE_UP -DDISABLE_END_MANAGER -DLOAD_FONT_BITMAP_FROM_FILE -Wall -Wextra -Wno-comment -Wno-unknown-pragmas --std=c++11 -g -IE:\Program_Files\DX_SDK\Include -I%D3D9_DIR%\inject -I%D3D9_DIR%\replace -I%D3D9_DIR%\game_specific -I%D3D9_DIR%\misc -I%D3D9_DIR%\libarchive

g++ resplit_lines.cpp ^
%D3D9_DIR%\game_specific\*.o %D3D9_DIR%\inject\*.o %D3D9_DIR%\replace\*.o %D3D9_DIR%\misc\*.o ^
-Wall -Wextra -Wno-comment -Wno-unknown-pragmas --std=c++11 -g ^
-IE:\Program_Files\DX_SDK\Include -I%D3D9_DIR%\inject -I%D3D9_DIR%\replace -I%D3D9_DIR%\game_specific -I%D3D9_DIR%\misc -I%D3D9_DIR%\libarchive ^
-lShlwapi -LE:\Program_Files\DX_SDK\Lib\x86 -lD3DX9 -L%D3D9_DIR%\libarchive -larchive_static

@del %D3D9_DIR%\game_specific\BohoCharBuff.o

@rem %D3D9_DIR%\replace\Char.cpp %D3D9_DIR%\replace\char_buff.cpp %D3D9_DIR%\replace\EndManager.cpp %D3D9_DIR%\replace\Log.cpp %D3D9_DIR%\replace\ParseText.cpp ^
@rem %D3D9_DIR%\game_specific\BohoCharBuff.cpp %D3D9_DIR%\game_specific\boho_init_chars.cpp ^
