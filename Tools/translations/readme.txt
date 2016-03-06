**********************************************************************
This readme explains how to edit the patch.
If you only want to use the patch, you don't need to read this readme,
and you should not touch to this folder.
Read the readme in the parent directory instead.
**********************************************************************

---------
Encodings
---------
Note about the files' encodings: games.ini and config.ini are read with the
Windows API, and so uses the local encoding. If you edit them with notepad,
save them with the "ANSI" encoding. In these files, you should only use ASCII
characters.
The other files uses UCS-2LE. If you edit them with notepad, save them with the
"Unicode" encoding.


---------
games.ini
---------
This file associates a game's MD5 hash with its name. The patch use these hashs
to know which game is running.
For now, the following games are supported :
- boho1 (Bouhou Koumakyou ~ Scarlet Falimy)
- boho1_config (config.exe for Bouhou Koumakyou ~ Scarlet Falimy)
- boho3 (Bouhou Koumakyou 2)
This file also lets you change the patch language. To do this this, in the line
with lang=en , replace "en" with another language name. You can put anything
here as long as the corresponding folder exists and the laguage name is shorter
than 20 characters.


----------
config.ini
----------
Each patch contains a config.ini file.
The config.ini file contains various settings regarding the patch. For a
complete description of the ini format itself, you can search on google.
The file is splitted in different sections. Each section begins with
[section name] (in brackets).

[global]
This section contains various settings.
Each value can be 1 (true) or 0 (false). 0 is assumed if the key is absent.
QWERTY:			Automatically sets the game's keymap to qwerty.
			It is useful for games assuming the keymap is QWERTY
			(Bouhou Koumakyou, for example).
			This is disabled under Windows 8 and later, because
			these systems doesn't keep a per-window keymap, so
			the keymap is changed for the entire system on these
			OSes. Please think about it if you decide to enable
			this setting.
SAVE_EMPTY_TEXTURES:	When the game creates an empty texture, this texture
			will be saved the first time it is displayed.
REMOVE_BACKGROUND:	Fills the screen with black just before printing the
			translations. This allows you to, for example, take a
			screenshot of some text without having anything else
			on the screenshot.

[Output]
This section allows you to see the patch logs. It has 2 settings.
Each value can be 1 (true) or 0 (false). 0 is assumed if the key is absent.
CONSOLE:		Enable logging to a console.
FILE:			Enable logging to a file (log.txt, in the
			translations directory).


The other sections contains internal configuration regarding some parts of the
patch. I'll describe them here.

[CharBuff]
The part which recognizes characters, stores them, then translates them.
Each value can be 1 (true) or 0 (false). 0 is assumed if the key is absent.
DISABLE_BMP_SAVE:	Don't save unknown characters as bitmaps.
DISABLE_CHARS_LIST:	Don't dump characters details into chars_dump.txt.
DISABLE_LOGS:		Combines DISABLE_BMP_SAVE and DISABLE_CHARS_LIST.
DISABLE_REPLACE_STR:	Disables the replacement of japanese strings by their
			translation.

[Log]
The part which saves the dialogs in the files dialogs_NNN.txt.
Each value can be 1 (true) or 0 (false). 0 is assumed if the key is absent.
HUMAN_READABLE_OUTPUT:	Output the logs in a more human-readable, but less easy
			to parse format (with field name before each value).
CREATE_NEW_FILE:	Create a new log file each time you launch the game
			(instead of erasing the old one).
DISABLE:		Disable logging through dialogs.txt
PRINT_ALL_CHARS:	After printing a sentence, print detailled logs for
			every character (by default, it prints detailled logs
			for unknown characters only).
PRINT_TRANSLATION:	After printing unknown characters, print the translation
			given by CharBuff.
HIDE_KNOWN_TEXTS:	Don't print texts with an english translation.

[ParseText]
A part of CharBuff, which take care of the translation.
This part has only one key called TYPE, which can take 2 values :
- SPLIT_FIRST:		Splits the text in lines, then tries to translate each
			line.
			Best when the texts still aren't known, because this
			will split the text into lines.
- TRANSLATE_FIRST:	Translate the beginning of the text, then display the
			untranslated part (if there is one) as is.
			Best when most of the texts are known, because the texts
			are taken by blocks instead of individual lines.
By default, TRANSLATE_FIRST is used.


[SelfTest]
Note: you should read the "Script files syntax" part before reading this part.
This patch engine can make some tests when the game starts, to ensure the
required files are presents, the files' syntax is correct, etc. It is done by
the self test.
The self test is useful for patch devlopment, but it should be disabled for a
public release.
If the key "enable" is present and set to 1, the self test will occur.
Otherwise, no testing will be done. Please note that the self test will open
a console, and log everything in this console as well as in the file
selftest_log.txt.

The self test can also check whether some lines overflows, depending of their
modifier (see "Script files syntax", 2nd paragraph).
- #none texts aren't checked.
- Each #center text should fit in a single line, between the coordinates given in
  the corresponding script file. If a #center text can't fit, the self test will
  report it.
- #left texts have automatic line breaks, so they can't overflow horizontally.
  To check if a #left text overflows, you have to specify how many lines it can
  use. To do this, insert in the SelfTest section of this file the corresponding
  #left modifier, then an equal sign, then the maximum number of lines for this
  #left modifier.
  For example:
  #left;275;615;20=2
  means texts with the #left;275;615;20 modifier can use 1 or 2 lines. If one of
  these texts needs 3 lines, the self test will report it.


[Textures]
This section lists the textures used by the game. Each line has the following
structure:
hash=flags:filename
(filename is optionnal)
For example:
365b1d3dfdc2671e57f746ae72e564a8=0x01000000:menu.png
or:
e2425f8d66ec20570c6cadfec75f1b63=0x02000000

hash is the hash of the texture. When a texture is dumped by the engine, its
filename is this hash.

flags is a bitmask of flags (in hexadecimal if you prefix it with 0x, in decimal
else). The flags 0x00000001 to 0x0000FFFF are defined by the engine, and the
flags 0x00010000 to 0xFFFF0000 can be defined by the various games.
The documentation for these flags can be found in the source code : in
engine/ATexturesManager.hpp for the flags defined by the engine, and in
game/<game_dir>/TexturesFlags.hpp for the game-specific flags.

filename can be used to replace the texture with the one pointed by filename
(in the patch directory). To take the same example as above, if
translations/boho1_en/config.ini contains the following line:
365b1d3dfdc2671e57f746ae72e564a8=0x01000000:menu.png
then the game will use translations/boho1_en/menu.png instead of the original
texture for 365b1d3dfdc2671e57f746ae72e564a8.
This feature can be used to translate the games's images.



------------
messages.txt
------------
This file contains various error messages used by the engine.
The syntax of this file is near of an ini file, but not exactly the same: there
isn't sections, and the file must be in unicode (while ini files use the current
code page).
The messages usually contains some variable parts. For example, in 'The
directory "%s" doesn't exists.', the name of the directory can change. You have
to put %s where some text is expected, and %d where a number is expected. You
can also use %% to put a percent sign. You can see the messages.txt file in
boho1_en for examples. An important note: the message you write must have the
same number of percent flags than the original, in the same order (expect %%,
which are ignored when counting the percent flags).
Also, if you want to put a line break, you can use \n. And to put a single
backclash, you can use \\.
If a message is missing, a default message is used. The messages.txt file in
the boho1_en directory is totally useless, because it contains the default
texts. If you remove it, the patch will use the default messages, which are the
same. I only put this file to give an example.


---------------------
new_files_message.txt
---------------------
Like the previous file, it contains a text to be displayed to the user. But the
corresponding text is mandatory and may contain lots of line breaks, so I
thought it would be easier to put it in aonther file.
This file contains the message displayed when some untranslated texts are
found by the patch. These texts should be sent to the translation team, so if
you do a patch using this patching tool, you should put in new_files_message.txt
everything the user needs to send you the untranslated files.
The syntax is quite different from the previous file: as this file contains
only one message, you don't need yo put its name before. You also don't need to
use \n for a line break : just put a normal line break. But the sentences
regarding the percent flags still apply to this file.




---------
chars.csv
---------
This file lists all the characters supported by a game using a bitmap font.
For more details about the CSV syntax, you can search on google.
Each line describes a character (except the 1st one, which gives the columns
names). Here is a detailled description of each column:
- c  : the described character. Note that this column is an exception to the
       CSV syntax : it must be exactly 1 character width, and it can be any
       character.
       If it is a coma, a quote, a backslash, or any other special character,
       you don't need to escape it.
- tx1: the x position of the character in the texture.
- tx2: the x position of the end of the character in the texture.
- ty1: the y position of the character in the texture.
- ty2: the y position of the end of the character in the texture.
- sy : the character will be displayed sy pixels under its normal position.
       Use -1 or 0 to ignore.
- sw : the character's width. -1 to ignore (this is discouraged if you want
       to display it).
- sh : the character's height. -1 to ignore (this is discouraged if you want
       to display it).



------------
Script files
------------
Each folder in this directory describes a patch. The folders' names are
composed of the patch's name, then an underscore, then the language name.
For example, the folder for the english patch for Bouhou Koumakyou (which
is called "boho1" internally) is boho1_en.
You can change the patch's language with the key "language" in config.ini.

In each folder, there is some files called script*.txt (where * can be
anything). These files associates an original text with its translation.
The file in which a text is doesn't matter. You can put everything in a file
called script.txt if you prefer to.
The file script_end.txt, if present, is a special file. It will be read at the
end, after the other files. The texts in this files will have the least
priority. If 2 texts begins the same way, you may want to put one of these 2 in
scrpit_end.txt.


-------------------
Script files syntax
-------------------
Each line of a script file associates an original string with a translated
string. You put the original string, then an equal sign, then the translation.
For example:
ゆっくリ装備を選んでね!!=Take your time picking your equipment, okay?
If a line doesn't need to be translated, you can put <NO TRANSLATION> as its
translation.
If you need to put an equal sign in the text, you can prefix it with a
backslash. If you need to put a backslash in the text, you can also prefix it
with a backslash.
If you want to put some notes in the translation files, you can prefix them with
2 sharps. For example :
## Here is a comment


By default, the translated text will be at the place the original text should
be put. You can apply a modifier, which will change the text position.
The available modifiers are:
- #left;x1;x2;h
Align the text to the left. x1 is the position (in pixels) where to put the
text.
This modifier enables automatic line breaks. If a word ends after x2
(in pixels), it is put instead at x1, h pixels under the previous line.
And the remaining of the translation is put on the right of this word.
Example :
#left;275;615;20

- #center;x1;x2
Center the texts between x1 and x2.
Example :
#center;0;640

- #none
Disables the current modifier.

When you put a modifier, it is applied on all following lines. Use #none to
stop using a modifier.
The files are read one after another, in a random order, without resetting
the modifier between files. So you should always put a modifier at the
beginning of a file (if you don't want to use a modifier, use #none).
