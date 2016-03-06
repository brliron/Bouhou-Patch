#ifndef DROPDOWN_H_
# define DROPDOWN_H_

# include	<Windows.h>

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */
int	DropdownBox(LPCWSTR title, LPCWSTR text, LPCWSTR* items);
# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* !DROPDOWN_H_ */
