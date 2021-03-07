//===================
// DeviceContext.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "DeviceContext.h"
#include "Font.h"

using GdiFont=Graphics::Gdi::Font;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

DeviceContext::DeviceContext():
hDeviceContext(NULL),
hOldTarget(NULL),
uFlags(DeviceContextFlags::Delete)
{
hDeviceContext=CreateCompatibleDC(NULL);
}

DeviceContext::DeviceContext(HDC hdc):
hDeviceContext(hdc),
hOldTarget(NULL),
uFlags(DeviceContextFlags::None)
{}

DeviceContext::~DeviceContext()
{
if(GetFlag(uFlags, DeviceContextFlags::Delete))
	DeleteDC(hDeviceContext);
}


//========
// Common
//========

VOID DeviceContext::BeginPaint(Handle<Bitmap> htarget)
{
Graphics::DeviceContext::BeginPaint(htarget);
if(htarget)
	{
	auto hbmp=htarget.As<GdiBitmap>();
	if(hbmp)
		hOldTarget=SelectObject(hDeviceContext, hbmp->GetHandle());
	}
}

VOID DeviceContext::Clear(COLOR c)
{
if(hTarget)
	{
	Graphics::DeviceContext::Clear(c);
	return;
	}
::RECT rc({ Clip.Left, Clip.Top, Clip.Right, Clip.Bottom });
HBRUSH hbr=CreateSolidBrush(c&0xFFFFFF);
::FillRect(hDeviceContext, &rc, hbr);
DeleteObject(hbr);
}

VOID DeviceContext::Clear(Handle<GdiBrush> hbr)
{
if(hTarget)
	{
	Graphics::DeviceContext::Clear(hbr->GetColor());
	return;
	}
::RECT rc({ Clip.Left, Clip.Top, Clip.Right, Clip.Bottom });
::FillRect(hDeviceContext, &rc, hbr->GetHandle());
}

VOID DeviceContext::DrawBitmap(Handle<GdiBitmap> hbmp, RECT const& rc)
{
if(!hbmp)
	return;
RECT rcc(rc);
if(!AdjustRect(rcc))
	return;
UINT uwidth=hbmp->GetWidth();
UINT uheight=hbmp->GetHeight();
HDC hdc=CreateCompatibleDC(hDeviceContext);
SelectObject(hdc, hbmp->GetHandle());
SetStretchBltMode(hDeviceContext, HALFTONE);
StretchBlt(hDeviceContext, rcc.Left, rcc.Top, rcc.Right-rcc.Left, rcc.Bottom-rcc.Top, hdc, 0, 0, uwidth, uheight, SRCCOPY);
DeleteDC(hdc);
}

VOID DeviceContext::DrawIcon(Handle<Icon> hico, INT ileft, INT itop)
{
if(!hico)
	return;
::DrawIcon(hDeviceContext, ileft, itop, hico->GetHandle());
}

VOID DeviceContext::DrawText(Handle<String> htext, COLOR c)
{
DrawText(htext, c, Clip);
}

VOID DeviceContext::DrawText(Handle<String> htext, COLOR c, RECT const& rc, UINT uformat)
{
if(!htext)
	return;
Handle<GdiFont> hfont=Font.As<GdiFont>();
if(!hfont)
	{
	Graphics::DeviceContext::DrawText(htext, c);
	return;
	}
RECT rcc(rc);
if(!AdjustRect(rcc))
	return;
::RECT rcd({ rcc.Left, rcc.Top, rcc.Right, rcc.Bottom });
SelectObject(hDeviceContext, hfont->GetHandle());
SetBkMode(hDeviceContext, TRANSPARENT);
SetTextColor(hDeviceContext, c);
::DrawText(hDeviceContext, htext->Begin(), htext->GetLength(), &rcd, uformat);
}

VOID DeviceContext::EndPaint()
{
if(hOldTarget)
	{
	SelectObject(hDeviceContext, hOldTarget);
	hOldTarget=NULL;
	}
}

VOID DeviceContext::FillRect(RECT const& rc, COLOR c)
{
if(hTarget)
	{
	Graphics::DeviceContext::Clear(c);
	return;
	}
RECT rcc(rc);
if(!AdjustRect(rcc))
	return;
::RECT rcf({ rcc.Left, rcc.Top, rcc.Right, rcc.Bottom });
HBRUSH hbr=CreateSolidBrush(c&0xFFFFFF);
::FillRect(hDeviceContext, &rcf, hbr);
DeleteObject(hbr);
}

VOID DeviceContext::FillRect(RECT const& rc, Handle<GdiBrush> hbr)
{
if(hTarget)
	{
	Graphics::DeviceContext::Clear(hbr->GetColor());
	return;
	}
RECT rcc(rc);
if(!AdjustRect(rcc))
	return;
::RECT rcf({ rcc.Left, rcc.Top, rcc.Right, rcc.Bottom });
::FillRect(hDeviceContext, &rcf, hbr->GetHandle());
}

SIZE DeviceContext::MeasureText(Handle<Graphics::Font> hfont, Handle<String> htext)
{
Handle<GdiFont> hfonte=hfont.As<GdiFont>();
if(!hfonte)
	return Graphics::DeviceContext::MeasureText(hfont, htext);
SIZE size(0, 0);
size.Height=hfonte->GetHeight();
if(!htext)
	return size;
SelectObject(hDeviceContext, hfonte->GetHandle());
::RECT rc;
ZeroMemory(&rc, sizeof(RECT));
::DrawText(hDeviceContext, htext->Begin(), htext->GetLength(), &rc, DT_CALCRECT);
size.Height=rc.bottom;
return size;
}

VOID DeviceContext::ReleaseTarget()
{
}

}}