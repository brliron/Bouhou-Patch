#ifndef MD5_HPP_
# define MD5_HPP_

# include	<Windows.h>

// Calculates the MD5 hash of some data.
void		md5(LPCVOID data, UINT size, char str[33]);
void		md5(LPCWSTR filename, char str[33]);

#endif /* !MD5_HPP_ */
