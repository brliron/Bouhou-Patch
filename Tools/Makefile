# Note regarding this makefile :
# Some of the tools in this folder needs a part of the patch engine. As I developp this patch, its different parts became more and more linked to each other.
# So the easiest way to use a part of the patch is to compile the entire patch.
# Therefore, the variables CPP, CPPFLAGS, LD, LDFLAGS (except the commented ones) and SRCS are an exact copy of the ones from the patch's Makefile.
# If you change these variables in the patch's Makefile, please change them here as well.

DIR1	=	1_update_chars_list
NAME1	=	$(DIR1)\a.exe
DIR2	=	2_update_dialogs
NAME2	=	$(DIR2)\a.exe
DIR3	=	3_resplit_lines
NAME3	=	$(DIR3)\a.exe

ROOT_DIR	=	../D3D9

CPP		=	g++
CPPFLAGS	+=	-Wall -Wextra -Wno-comment -Wno-unknown-pragmas -g -std=c++11
CPPFLAGS	+=	-IE:\Program_Files\DX_SDK\Include -I$(ROOT_DIR) -I$(ROOT_DIR)\game -I$(ROOT_DIR)\engine -I$(ROOT_DIR)\patch -I$(ROOT_DIR)\misc -I$(ROOT_DIR)\libarchive
CPPFLAGS	+=	-DDISABLE_DRAW_PRIMITIVE_UP -DDISABLE_END_MANAGER -DDISABLE_BMP -DDISABLE_ARCHIVE -DENABLE_BOHO

LD		=	g++
LDFLAGS		+=	-Wl,--enable-stdcall-fixup
#LDFLAGS		+=	-Wl,--kill-at -shared
LDFLAGS		+=	-lShell32 -lShlwapi -lOle32 -lOleAut32 -lgdi32 -lcomctl32 C:\Windows\SysWoW64\D3DX9_33.dll -L$(ROOT_DIR)\libarchive -larchive_static
#LDFLAGS		+=	-Bstatic -static-libgcc -static-libstdc++

SRCS	=	$(ROOT_DIR)/engine/main.cpp \
		$(ROOT_DIR)/engine/MyD3D9.cpp \
		$(ROOT_DIR)/engine/D3DDevice9.cpp \
		$(ROOT_DIR)/engine/ATexturesManager.cpp \
		$(ROOT_DIR)/engine/AD3DTexturesManager.cpp \
		$(ROOT_DIR)/patch/char_buff.cpp \
		$(ROOT_DIR)/patch/ParseText.cpp \
		$(ROOT_DIR)/patch/Char.cpp \
		$(ROOT_DIR)/patch/Log.cpp\
		$(ROOT_DIR)/patch/move_texts.cpp \
		$(ROOT_DIR)/patch/EndManager.cpp \
		$(ROOT_DIR)/patch/Reader.cpp \
		$(ROOT_DIR)/patch/Output.cpp\
		$(ROOT_DIR)/patch/BackgroundTasksManager.cpp \
		$(ROOT_DIR)/misc/Bitmap.cpp \
		$(ROOT_DIR)/misc/Bitmap_load.cpp \
		$(ROOT_DIR)/misc/Archive.cpp \
		$(ROOT_DIR)/misc/md5.cpp \
		$(ROOT_DIR)/misc/md5_algo.cpp \
		$(ROOT_DIR)/misc/chcp.cpp \
		$(ROOT_DIR)/misc/DropdownBox.cpp \
		$(ROOT_DIR)/game/AD3DPatch.cpp \
		$(ROOT_DIR)/game/None/Patch.cpp

APATCH_SRC =	$(ROOT_DIR)/game/APatch.cpp

BOHO_SRCS =	$(ROOT_DIR)/game/Boho/APatch.cpp \
		$(ROOT_DIR)/game/Boho/1/Patch.cpp \
		$(ROOT_DIR)/game/Boho/3/Patch.cpp \
		$(ROOT_DIR)/game/Boho/D3DDevice9.cpp \
		$(ROOT_DIR)/game/Boho/1/D3DDevice9.cpp \
		$(ROOT_DIR)/game/Boho/3/D3DDevice9.cpp \
		$(ROOT_DIR)/game/Boho/TexturesManager.cpp \
		$(ROOT_DIR)/game/Boho/CharBuff.cpp \
		$(ROOT_DIR)/game/BohoConfig/APatch.cpp \
		$(ROOT_DIR)/game/BohoConfig/Boho1Patch.cpp

TOOLS_SRCS =	$(SRCS) \
		$(APATCH_SRC) \
		$(BOHO_SRCS) \
		init.cpp

SRCS1	=	$(TOOLS_SRCS) \
		$(DIR1)\update_chars_list.cpp

SRCS2	=	$(TOOLS_SRCS) \
		$(DIR2)\update_dialogs.cpp

SRCS3	=	$(TOOLS_SRCS) \
		$(DIR3)\resplit_lines.cpp

OBJS1	=	$(SRCS1:.cpp=.o)
OBJS2	=	$(SRCS2:.cpp=.o)
OBJS3	=	$(SRCS3:.cpp=.o)

all: $(NAME1) $(NAME2) $(NAME3)

$(NAME1): $(OBJS1)
	$(LD) $(OBJS1) $(LDFLAGS) -o $(NAME1)

$(NAME2): $(OBJS2)
	$(LD) $(OBJS2) $(LDFLAGS) -o $(NAME2)

$(NAME3): $(OBJS3)
	$(LD) $(OBJS3) $(LDFLAGS) -o $(NAME3)

clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3)

fclean: clean
	rm -f $(NAME1) $(NAME2) $(NAME3)

re: fclean all
