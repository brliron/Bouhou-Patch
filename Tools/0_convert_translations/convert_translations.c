#include	<stdio.h>
#include	<Windows.h>

  static const LPCWSTR	strings[] =
    {
// Loading stage 1
// Patchouli :
L"気を使う程度の能カ、か……。",
L"The ability to control Ki, huh...",
//Ki or Qi?

// Sakuya :
L"美鈴のことですね。どうかしました?",
L"You are speaking of Meiling, right? Did something happen?",

// Patchouli :
L"いえ、言いえて妙と思って。大概の侵入者は彼",
L"No, I just thought it was a little strange.",
L"女の見張る門を通る時、",
L"When most of the intruders get past her,",


L"こんなにあっさリ通過していいものかどうか、戸",
L"I wonder whether it is a good thing that",
L"惑ってしまうもの。",
L"everyone can get past her so easily.",

// Sakuya :
L"あ、ああ……。",
L"...I see...",

// Loading stage 2
// Patchouli :
L"門番を変えた方がいいんじゃないかと思うの。",
L"I feel like we should get a new Gate Guard.",

// Sakuya :
L"はあ、いずれどなたかが仰るとは思っていました",
L"I figured someone would bring it up sooner or later, but...",
L"が……。", NULL,

// Remilia :
L"そうね、もう少し真面目に務める者が誰かい",
L"True, it would be nice if there was someone who",
L"ればいいのだけれど。",
L"took their work a little more seriously.",

// Sakuya :
L"お言葉ですが、彼女はあれで中々のッワモノで\nす。",
L"It is as you said, but she is rather strong in her own way.",
//Attempting some preempting here, not sure whether it'll display properly though
//The "tsu" character here (ッ) is not actually the correct character.  It's a smaller version of the real character (ツ） which is used chiefly as a diacritic, not an actual pronouncable sound.  I'm not sure whether this is a transcription error or an error on the part of the developer, so I'll include the corrected version below in comments. If it doesn't work properly, try switching it out.

//L"お言葉ですが、彼女はあれで中々のツワモノで
//す。",
//L"It is as you said, but she is rather strong in her own way.",


L"あっさリ切ってしまうのは勿体無いのでは……。",
L"It would be a waste to fire her so easily...",

// Patchouli :
L"ッワモノってどのくらい?",
L"What do you mean \"in her own way?\"",

//Same issue as above.
//L"ツワモノってどのくらい?",
//L"What do you mean \"in her own way?\"",

// Sakuya :
L"かつて無数の矢を受け、決して地に臥す事無",
L"There are some who would take countless arrows without",
L"く息絶えた武人がいましたが、",
L"even laying down to die, but",

L"彼女の場合はきっと、決して地に臥す事無く",
L"in her case, she doesn't even lie down",
L"眠リ続けるでしょう。",
L"to sleep.",

// Remilia :
L"……え、わかリづらい。",
L"...I don't really get it.",

// Loading stage 1
// Remilia :
L"このグローブ、フランが使えなくて良かったわ…\n…。",
L"It's a good thing Flan didn't try using this glove...",

// Sakuya :
L"……想像してみましたけど、うわあ……。",
L"...even just trying to imagining it is scary...",

// Patchouli :
L"……想像したけど、うわあ……。",
L"...even just imagining it is scary...",
//Very similar to Sakuya's line. Not sure if this will be a joke that doesn't translate well, or if only one of these show up at a time.

// Loading stage 1
// Sakuya :
L"もし、もしですよお嬢様。",
L"If, say, for example, mistress...",

// Remilia :
L"え、何よ咲夜。",
L"Hm? What is it, Sakuya?",

// Sakuya :
L"フランお嬢様に、もし素敵な殿方が現れて。お",
L"What if a beautiful new mansion was to appear for Flandre?",
L"嫁に出るような事があれば、",
L"If she were able to leave the house,",
L"それで皆が幸せなんじゃないでしょうか。",
L"I was thinking then everyone would be happier.",

// Remilia :
L"そうね……キレイな花火が上がリそうね。",
L"I think you're right...we'd all be able to see some fantastic fireworks.",

// Sakuya :
L"……はい、忘れて下さい。",
L"...please forget what I just said.",

// Loading stage 1
// Meiling :
L"パチュリー様、この\"ブースター\"というのは何な",
L"Miss Patchouli, what is this \"Booster\"?",
L"のですか?", NULL,

// Patchouli :
L"危なっかしさに拍車をかけるモノ、という意味\nよ。",
L"It's something that encourages danger.",

// Meiling :
L"ああ、そういう……。",
L"Ahh, I see...",

// Loading stage 1
// Meiling :
L"咲夜さん見てください、このグローブさえあれば",
L"Sakuya, look at this! With this glove, I have",
L"百人カですよ!",
L"The strength of a hundred people!",

// Loading stage 6
// Meiling :
//咲夜さん見てください、このグローブさえあれば
//百人カですよ!
//Duplicate

// Sakuya :
L"中々いいものを手に入れたみたいね。",
L"It looks like you've found something nice.",

// Meiling :
L"ええ、これさえあれば……",
L"With this...",

L"対侵入者用の落とし穴を、いくらでも掘ってお\nけます!",
L"I can dig any number of holes to trap intruders!",

// Sakuya :
L"……穴があったらぶちこみたいとはこの事かしら\n……。",
L"...\"Wanting to push someone into a hole if it exists\" is something like this, huh?",

// Loading stage 6
// Patchouli :
L"あら、ちょうどいいものを手に入れたじゃない、\n美鈴。",
L"Hm, it looks like you found something handy, Meiling.",

// Meiling :
L"パチュリー様。ちょうどいい、とは?",
L"What do you mean by \"handy,\" Miss Patchouli?",

// Patchouli :
L"そのグローブで土を耕して、咲夜の庭の手入れ",
L"Use that glove to move dirt,",
L"を手伝ってあげなさい。",
L"and go help Sakuya out in her garden.",

// Meiling :
L"私はもぐらですか!",
L"What am I, a mole?!",

// Patchouli :
L"そうね、夜行性のようだし。",
L"You are nocturnal like one.",

// Meiling :
L"そんな、日中の務めに支障が出ないよう、夜は",
L"That's not true at all! I make sure to sleep well every night",
L"しっかリ寝ています!",
L"to ensure there will be no problems during the day!",

// Patchouli :
L"……。",
L"..........",

// Loading stage 6
// Sakuya :
L"あの土の迷宮に、お嬢様やフラン様が迷い込",
L"It's a good thing Miss Flandre and the Mistress didn't end up",
L"んでしまわなくて良かったです。",
L"getting lost in that Labyrinth.",

// Patchouli :
L"そうね、ホコリっぽいのは好きではないでしょうし\n。",
L"I agree. They don't like dusty places at all.",

// Sakuya :
L"ええ。美鈴のように、通る所を掘リ返すだけで",
L"Yes, just like Meiling, they wouldn't be satisfied until",
L"は済まなかったでしょうから。",
L"they had torn up the entire place.",

// Patchouli :
L"……地層ごと消し飛ばしかねないわね。",
L"...they might have blown away the whole level.",

// Loading stage 5
// Flandre :
L"さっき、高いところにキレイな箱が置いてあるの",
L"Earlier, I saw a pretty box placed really high up.",
L"をみつけたの。", NULL,

L"お姉様なら届くんじゃないかと思うわ。",
L"I thought, I bet Oneesama could reach that.",

L"後で帰ってきたら教えてあげるの!",
L"When she comes back, I'll tell her about it!",

// Patchouli :
L"(何……何かのワナなの……?)",
L"(Is...is this some sort of trap...?)",

      NULL
    };

int	escape_chars(LPWSTR str)
{
  int	i;

  i = 0;
  while (str[i])
    {
      if (str[i] == L'=' || str[i] == L'\n')
	{
	  if (wcslen(str) >= 255)
	    return 0;
	  if (str[i] == L'\n')
	    str[i] = L'n';
	  memmove(str + i + 1, str + i, (wcslen(str + i) + 1) * 2);
	  str[i] = L'\\';
	}
      i++;
    }
  return 1;
}

int	main()
{
  FILE*	fd;
  WCHAR	s1[256];
  WCHAR	s2[256];
  int	i;

  fd = fopen("out.txt", "w, ccs=UNICODE");
  for (i = 0; strings[i] != NULL; i += 2)
    {
      if (strings[i][0] == L'#')
	{
	  fwprintf(fd, L"%s\n", strings[i]);
	  continue ;
	}
      if (wcslen(strings[i]) >= 256 || (strings[i + 1] && wcslen(strings[i + 1]) >= 256))
	{
	  printf("Error : string %d or %d is too long (256 characters max)\n", i, i + 1);
	  continue ;
	}
      wcscpy(s1, strings[i]);
      if (strings[i + 1])
	wcscpy(s2, strings[i + 1]);
      else
	wcscpy(s2, L"<NO TRANSLATION>");
      if (escape_chars(s1) == 0 || escape_chars(s2) == 0)
	{
	  printf("Error : string %d or %d became too long while escaping a spacial character (256 characters max)\n", i, i + 1);
	  continue ;
	}
      fwprintf(fd, L"%s=%s\n", s1, s2);
    }
  fclose(fd);
  return 0;
}
