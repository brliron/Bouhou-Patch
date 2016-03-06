# The easyest way to run this Makefile is to use the "all" rule, by calling the make utility without arguments.
# If you don't want to include some game engines, run the "core" rule, then the rules related to the games you want to build, then the "link" rule.
# For example : make core boho link

NAME	=	D3D9.dll

ROOT_DIR	=	.

# Some functionnalities can be enabled / disabled by defining some macros in the CPPFLAGS.
# DISABLE_DRAW_PRIMITIVE_UP : Disables the use of the D3DDevice9::DrawPrimitiveUp function (it is used by some tools, but can probably be repladed by using None::CharBuff).
# DISABLE_END_MANAGER	    : Disables the use of the EndManager class.
# DISABLE_BMP		    : Disables the saving of unknown characters as BMP files.
# DISABLE_ARCHIVE	    : With this flag, the files created by the patch are stored on the disk as files, not in an archive.
# ENABLE_BOHO		    : Enables the support of Boho games in game/Apatch.cpp . Automatically enabled by the "boho" rule.

CPP		=	g++
CPPFLAGS	+=	-Wall -Wextra -Wno-comment -Wno-unknown-pragmas -std=c++11
DEBUG		=	0
ifeq ($(DEBUG), 1)
  CPPFLAGS	+=	-g
endif
CPPFLAGS	+=	-IE:\Program_Files\DX_SDK\Include -I$(ROOT_DIR) -I$(ROOT_DIR)\game -I$(ROOT_DIR)\engine -I$(ROOT_DIR)\patch -I$(ROOT_DIR)\misc -I$(ROOT_DIR)\libarchive

LD		=	g++
LDFLAGS		+=	-Wl,--enable-stdcall-fixup
LDFLAGS		+=	-Wl,--kill-at -shared
LDFLAGS		+=	-lShell32 -lShlwapi -lOle32 -lOleAut32 -lgdi32 -lcomctl32 C:\Windows\SysWoW64\D3DX9_33.dll -L$(ROOT_DIR)\libarchive -larchive_static
LDFLAGS		+=	-Bstatic -static-libgcc -static-libstdc++

ifeq ($(TERM), cygwin)
  CP	=	cp
  RM	=	rm
  RM_END=	> /dev/null 2> /dev/null || true
else
  CP	=	copy >nul
  RM	=	del
  RM_END=	>nul 2>nul || ver > nul
endif

SRCS	=	$(ROOT_DIR)\engine\main.cpp \
		$(ROOT_DIR)\engine\MyD3D9.cpp \
		$(ROOT_DIR)\engine\D3DDevice9.cpp \
		$(ROOT_DIR)\engine\ATexturesManager.cpp \
		$(ROOT_DIR)\engine\AD3DTexturesManager.cpp \
		$(ROOT_DIR)\patch\char_buff.cpp \
		$(ROOT_DIR)\patch\ParseText.cpp \
		$(ROOT_DIR)\patch\Char.cpp \
		$(ROOT_DIR)\patch\Log.cpp\
		$(ROOT_DIR)\patch\move_texts.cpp \
		$(ROOT_DIR)\patch\EndManager.cpp \
		$(ROOT_DIR)\patch\Reader.cpp \
		$(ROOT_DIR)\patch\Output.cpp\
		$(ROOT_DIR)\patch\BackgroundTasksManager.cpp \
		$(ROOT_DIR)\misc\Bitmap.cpp \
		$(ROOT_DIR)\misc\Bitmap_load.cpp \
		$(ROOT_DIR)\misc\Archive.cpp \
		$(ROOT_DIR)\misc\md5.cpp \
		$(ROOT_DIR)\misc\md5_algo.cpp \
		$(ROOT_DIR)\misc\chcp.cpp \
		$(ROOT_DIR)\misc\DropdownBox.cpp \
		$(ROOT_DIR)\game\AD3DPatch.cpp \
		$(ROOT_DIR)\game\None\Patch.cpp

APATCH_SRC =	$(ROOT_DIR)\game\APatch.cpp

BOHO_SRCS =	$(ROOT_DIR)\game\Boho\APatch.cpp \
		$(ROOT_DIR)\game\Boho\1\Patch.cpp \
		$(ROOT_DIR)\game\Boho\2\Patch.cpp \
		$(ROOT_DIR)\game\Boho\3\Patch.cpp \
		$(ROOT_DIR)\game\Boho\D3DDevice9.cpp \
		$(ROOT_DIR)\game\Boho\1\D3DDevice9.cpp \
		$(ROOT_DIR)\game\Boho\2\D3DDevice9.cpp \
		$(ROOT_DIR)\game\Boho\3\D3DDevice9.cpp \
		$(ROOT_DIR)\game\Boho\TexturesManager.cpp \
		$(ROOT_DIR)\game\Boho\CharBuff.cpp \
		$(ROOT_DIR)\game\BohoConfig\APatch.cpp \
		$(ROOT_DIR)\game\BohoConfig\Boho1Patch.cpp \
		$(ROOT_DIR)\game\BohoConfig\Boho2Patch.cpp \
		$(ROOT_DIR)\game\D3DTextures\Patch.cpp

OBJS	=	$(SRCS:.cpp=.o)

APATCH_OBJ =	$(APATCH_SRC:.cpp=.o)

BOHO_OBJS =	$(BOHO_SRCS:.cpp=.o)

all: core boho link

core: $(OBJS)
	$(RM) $(APATCH_OBJ) $(RM_END)
# We remove APATCH_OBJ because it may have been compiled with different flags the last time.

boho: $(BOHO_OBJS)
	$(eval OBJS += $(BOHO_OBJS))
	$(eval CPPFLAGS += -DENABLE_BOHO)

link: $(APATCH_OBJ)
	$(LD) $(OBJS) $(APATCH_OBJ) $(LDFLAGS) $(ROOT_DIR)\engine\D3D9.def -o $(NAME)

#	@cp $(NAME) "C:\Users\liron_b\Desktop\Boho Koumakyou"
#	echo 'cd "C:\Users\liron_b\Desktop\Boho Koumakyou" && ./tkf.exe && true' | sh &
####	cmd /C "cd "C:\Users\liron_b\Desktop\Boho Koumakyou - Copie (tests injection D3DX9)" && tkf.exe"

#	@$(CP) $(NAME) "C:\Users\liron_b\Desktop\Boho Youyoumu"
####	echo 'cd "C:\Users\liron_b\Desktop\Boho Youyoumu" && ./boho2.exe && true' | sh &
#	cmd /C "cd "C:\Users\liron_b\Desktop\Boho Youyoumu" && boho2.exe || exit -b 0"

#	@cp $(NAME) "C:\Users\liron_b\Downloads\boho\Touhou Koumakyou - Scarlet Family 2"
#	echo 'cd "C:\Users\liron_b\Downloads\boho\Touhou Koumakyou - Scarlet Family 2" && ./boho3.exe && true' | sh &

clean:
	$(RM) $(OBJS) $(BOHO_OBJS) $(APATCH_OBJ) $(RM_END)

fclean: clean
	$(RM) $(NAME) $(RM_END)

re: fclean all
