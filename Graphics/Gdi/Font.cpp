//==========
// Font.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Font::Font(Fonts font):
Graphics::Font("Graphics.Gdi.Font"),
hFont(NULL)
{
if(!GetInfo(font, cInfo))
	return;
hFont=CreateFontIndirect(&cInfo);
if(hFont==INVALID_HANDLE_VALUE)
	{
	ZeroMemory(&cInfo, sizeof(LOGFONT));
	hFont=NULL;
	}
}

Font::Font(LOGFONT const& lf):
Graphics::Font("Graphics.Gdi.Font"),
hFont(NULL)
{
CopyMemory(&cInfo, &lf, sizeof(LOGFONT));
hFont=CreateFontIndirect(&cInfo);
if(hFont==INVALID_HANDLE_VALUE)
	{
	ZeroMemory(&cInfo, sizeof(LOGFONT));
	hFont=NULL;
	}
}

Font::Font(LPCSTR pfamily, UINT uheight, UINT uweight):
Graphics::Font("Graphics.Gdi.Font"),
hFont(NULL)
{
ZeroMemory(&cInfo, sizeof(LOGFONT));
StringCopy(cInfo.lfFaceName, 32, pfamily);
cInfo.lfHeight=uheight;
cInfo.lfWeight=uweight;
hFont=CreateFontIndirect(&cInfo);
if(hFont==INVALID_HANDLE_VALUE)
	{
	ZeroMemory(&cInfo, sizeof(LOGFONT));
	hFont=NULL;
	}
}

Font::~Font()
{
if(hFont)
	DeleteObject(hFont);
}


//========
// Common
//========

BOOL Font::GetInfo(Fonts font, LOGFONT& lf)
{
ZeroMemory(&lf, sizeof(LOGFONT));
NONCLIENTMETRICS ncm;
ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
ncm.cbSize=sizeof(NONCLIENTMETRICS);
SetLastError(0);
if(!SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0))
	return false;
LOGFONT const* plf=nullptr;
switch(font)
	{
	case Fonts::CaptionFont:
		{
		plf=&ncm.lfCaptionFont;
		break;
		}
	case Fonts::HyperLinkFont:
		{
		CopyMemory(&lf, &ncm.lfMenuFont, sizeof(LOGFONT));
		lf.lfUnderline=1;
		return true;
		}
	case Fonts::MenuFont:
		{
		plf=&ncm.lfMenuFont;
		break;
		}
	case Fonts::MessageFont:
		{
		plf=&ncm.lfMessageFont;
		break;
		}
	case Fonts::SmallCaptionFont:
		{
		plf=&ncm.lfSmCaptionFont;
		break;
		}
	case Fonts::StatusFont:
		{
		plf=&ncm.lfStatusFont;
		break;
		}
	default:
		{
		return false;
		}
	}
CopyMemory(&lf, plf, sizeof(LOGFONT));
return true;
}

}}