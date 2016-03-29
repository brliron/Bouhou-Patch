#ifndef CHARS_HPP_
# define CHARS_HPP_

# include	<vector>
# include	<list>
# include	"D3dx9core.h"


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
int	move_texts(const std::vector<LPCWSTR>& strings, int idx, std::list<DisplayedChar*>::iterator begin, std::list<DisplayedChar*>::iterator end);

#endif /* !CHARS_HPP_ */
