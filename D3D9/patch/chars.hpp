#ifndef CHARS_HPP_
# define CHARS_HPP_

# include	<string>
# include	<vector>
# include	<list>
# include	"D3dx9core.h"
# include	"EndManager.hpp"


struct		TLVERTEX
{
  float		x;
  float		y;
  float		z;
  float		rhw;
  D3DCOLOR	colour;
  float		u;
  float		v;
};

/*
** Note about the filed names (tx1, tx2...):
** - The 1st letter is t if the field refers to a texture file, or s if it is a on-screen coordinate.
** - The 2nd letter can be :
**   - x for the X position ;
**   - y for the Y position ;
**   - w for the width ;
**   - h for the height.
** - If a x or y character is followed by a number :
**   - x1 is the same as x ;
**   - x2 contains x1 + w ;
**   - y1 is the same as y ;
**   - y2 contains y1 + h ;
*/
struct	Char
{
private:
  bool		store(int flags);
  void		store_as_bmp() const;
  bool		update_from_bmp_filename(int flags);
  void		update(const Char& src);

public:
  WCHAR		c;
  float		tx1;
  float		tx2;
  float		ty1;
  float		ty2;
  int		sy;
  int		sw;
  int		sh;

  void		log(int flags);
  bool		operator<(const Char& c) const;
  bool		operator==(const Char& c) const;
  bool		operator!=(const Char& c) const;
  bool		cmp_by_pos(float tx1, float ty1) const;
  bool		cmp_by_pos(const Char& c) const;

  static Char*	get(wchar_t c);
  static Char*	get(float u, float v);
  static void	print_logs_to_file();
};


struct	DisplayedChar : public Char
{
  int	dst_x1;
  int	dst_x2;
  int	dst_y1;
  int	dst_y2;

  const DisplayedChar&	operator=(const Char& src);
  int			get_abs_y() const;
  bool			is_on_line(int line_y) const;
};

bool	float_cmp(float f1, float f2);



// TODO: replace these defines with static const variables in ACharBuff.
# define	CHARBUFF__NO_FLAGS		0x00
# define	CHARBUFF__DISABLE_BMP_SAVE	0x01	// Don't save unknown characters as bitmaps
# define	CHARBUFF__DISABLE_CHARS_LIST	0x02	// Don't dump characters details into chars_dump.txt
# define	CHARBUFF__DISABLE_LOGS		(CHARBUFF__DISABLE_BMP_SAVE & CHARBUFF__DISABLE_CHARS_LIST)
# define	CHARBUFF__DISABLE_REPLACE_STR	0x04	// Disables the replacing of japanese strings by their translation


/*
** Store all the characters displayed by the game with the add_char function, and displays the translated version with dump_str.
** Each patch must implement the put_char functions to display a character on the screen.
*/
// TODO: move in a file called CharBuff.hpp, and rename char_buff.cpp to CharBuff.cpp.
class	ACharBuff : public EndManager::Object
{
private:
  int		flags;

  virtual void	put_char(const Char& c, int x1, int x2, int y1, int y2) = 0;
  virtual void	put_char(const Char& c, int x, int y) = 0;
  virtual void	put_char(const DisplayedChar& c) = 0;
  void	put_str(const wchar_t* str, int x, int y);
  void	dump_line(const std::wstring& str, std::list<DisplayedChar*>& cur_buff);

protected:
  std::list<DisplayedChar*>	buff;

public:
  ACharBuff();
  ACharBuff(std::list<DisplayedChar*>& buff, int flags = CHARBUFF__NO_FLAGS);
  virtual ~ACharBuff();

  void		add_char(Char* c, float x1, float x2, float y1, float y2);
  virtual void	dump_str();
  bool		empty() const;
};


int	move_texts(const std::vector<LPCWSTR>& strings, int idx, std::list<DisplayedChar*>::iterator begin, std::list<DisplayedChar*>::iterator end);


#endif /* !CHARS_HPP_ */
