#ifndef BOHOCONFIG_APATCH_HPP_
# define BOHOCONFIG_APATCH_HPP_

# include	"game/APatch.hpp"

/*
** Implements the APatch class for the config programs of the Bouhou games (Bouhou Koumakyou, Bouhou Youyoumu and Bouhou Koumakyou 2).
** This class should be generic enough to handle the 3 games, and probably other unrelated games. At least that's what I want it to be.
** I think it currently works only for Bouhou Koumakyou, but I'll change that in the future.
*/

namespace	BohoConfig
{
  class	APatch : public ::APatch
  {
  private:
    ATexturesManager*		newTexturesManager() const;
    ACharBuff*			newCharBuff(int flags = CHARBUFF__NO_FLAGS) const;

    static BOOL WINAPI		SendMessageA(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    static BOOL WINAPI		SetWindowTextA(HWND hwnd, LPCSTR lpString);
    static LPCWSTR		getMatch(LPCWSTR str);
    static void			adjustSlider(HWND hText, HWND hSlider);
    static BOOL CALLBACK	enumProc(HWND hwnd, LPARAM lParam);

  public:
    APatch();
    void			translateWindows(HWND hwnd);
    bool			isGameExiting(CWPSTRUCT* msgParams) const;
  };
}

#endif /* !BOHOCONFIG_APATCH_HPP_ */
