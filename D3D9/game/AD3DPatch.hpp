#ifndef ID3DPATCH_HPP_
# define ID3DPATCH_HPP_

# include	"APatch.hpp"
# include	"D3DDevice9.hpp"
# include	"AD3DTexturesManager.hpp"

/*
** The Patch class of a Direct3D9-based game must implement this interface.
*/
class	AD3DPatch : public APatch
{
private:
  bool			isFullscreen;

protected:
  D3DDevice9*		D3DDevice;

  virtual D3DDevice9*	newD3DDevice() const = 0;

public:
  AD3DPatch();
  virtual ~AD3DPatch();

  // Return the instances of D3D9::TexturesManager and D3DDevice9 associated with AD3DPatch, allocating them if needed.
  D3D9::ATexturesManager*	getD3D9TexturesManager();
  D3DDevice9*			getD3DDevice();
  // Calls APatch::get, and casts the result to AD3DPatch.
  static AD3DPatch*		get();

  void				MessageBox(LPCWSTR text, LPCWSTR title = NULL) const;
  virtual void			initD3D(HWND hFocusWindow, D3DPRESENT_PARAMETERS* pPresentationParameters);

  // Return true if the global APatch instance is already allocated and is an instance of AD3DPatch, false otherwise.
  static bool			hasInstance();
};

#endif /* !ID3DPATCH_HPP_ */
