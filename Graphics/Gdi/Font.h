//========
// Font.h
//========

#pragma once


//=======
// Using
//=======

#include "Graphics/Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//=======
// Fonts
//=======

enum class Fonts: UINT
{
CaptionFont,
HyperLinkFont,
MenuFont,
MessageFont,
SmallCaptionFont,
StatusFont
};


//======
// Font
//======

class Font: public Graphics::Font
{
public:
	// Con-/Destructors
	Font(Fonts Font);
	Font(LOGFONT const& Info);
	Font(LPCSTR Family="Segoe UI", UINT Height=15, UINT Weight=500);
	~Font();

	// Common
	HFONT GetHandle()const { return hFont; }
	WORD GetHeight()const override { return (WORD)cInfo.lfHeight; }
	LOGFONT const& GetInfo() { return cInfo; }
	static BOOL GetInfo(Fonts Font, LOGFONT& Info);

private:
	// Common
	LOGFONT cInfo;
	HFONT hFont;
};

}}