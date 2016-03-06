#include	<Windows.h>
#define		protected	public // Because this is a tool, not the patch. In the patch, we should use APatch::getInstance(), but here, I can't rely on APatch::getInstance() to set APatch::instance correctly.
#include	"APatch.hpp"
#include	"Boho/1/Patch.hpp"
#include	"Boho/3/Patch.hpp"

bool	init()
{
  APatch::instance = new Boho1::Patch();
  // APatch::instance = new Boho3::Patch();
  if (APatch::instance->init() == false)
    return false;
  return true;
}
