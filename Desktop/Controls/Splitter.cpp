//==============
// Splitter.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include <windowsx.h>
#include "Splitter.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Splitter::Splitter(Handle<Container> hparent, SplitterOrientation orientation):
CoreWindow(hparent),
CoreControl(hparent),
CoreContainer(hparent),
Container(hparent),
cOrientation(orientation),
iCurrentSize(0),
iMaxSize0(0),
iMaxSize1(0),
iMinSize0(0),
iMinSize1(0),
iSize(4),
iStartSize(0),
ptStart({0, 0})
{
MessageReceived.Add(this, &Splitter::OnMessageReceived);
WindowInfo info;
info.Style=WS_CLIPCHILDREN|WS_VISIBLE;
info.StyleEx=WS_EX_CONTROLPARENT;
if(cOrientation==SplitterOrientation::Horizontal)
	{
	info.Class="SplitterH";
	info.Cursor=new Graphics::Gdi::Cursor(Cursors::SizeNS);
	}
else
	{
	info.Class="SplitterV";
	info.Cursor=new Graphics::Gdi::Cursor(Cursors::SizeWE);
	}
Create(info);
}


//========
// Common
//========

VOID Splitter::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Container::Rearrange(hdc, rc);
Handle<CoreControl> hctl0;
Handle<CoreControl> hctl1;
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	if(!hctl0)
		{
		hctl0=hchild;
		continue;
		}
	hctl1=hchild;
	break;
	}
if(!hctl0)
	return;
if(!hctl1)
	{
	hctl0->Move(rc);
	return;
	}
SIZE minsize0=hctl0->GetMinSize(hdc);
SIZE minsize1=hctl1->GetMinSize(hdc);
if(cOrientation==SplitterOrientation::Horizontal)
	{
	iMaxSize0=hctl0->MaxSize.Height;
	iMaxSize1=hctl1->MaxSize.Height;
	iMinSize0=MAX(minsize0.Height, 10);
	iMinSize1=MAX(minsize1.Height, 10);
	}
else
	{
	iMaxSize0=hctl0->MaxSize.Width;
	iMaxSize1=hctl1->MaxSize.Width;
	iMinSize0=MAX(minsize0.Width, 10);
	iMinSize1=MAX(minsize1.Width, 10);
	}
RECT const& rc0=hctl0->GetRect();
if(cOrientation==SplitterOrientation::Horizontal)
	{
	if(!iCurrentSize)
		iCurrentSize=rc0.Bottom-rc0.Top;
	RECT rcc(rc);
	rcc.Bottom=MIN(iCurrentSize, rc.Bottom-iSize-iMinSize1);
	hctl0->Move(rcc);
	rcc.Top=rcc.Bottom+iSize;
	rcc.Bottom=rc.Bottom;
	hctl1->Move(rcc);
	}
else
	{
	if(!iCurrentSize)
		iCurrentSize=rc0.Right-rc0.Left;
	RECT rcc(rc);
	rcc.Right=MIN(iCurrentSize, rc.Right-iSize-iMinSize1);
	hctl0->Move(rcc);
	rcc.Left=rcc.Right+iSize;
	rcc.Right=rc.Right;
	hctl1->Move(rcc);
	}
}


//================
// Common Private
//================

VOID Splitter::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(bhandled)
	return;
bhandled=true;
switch(umsg)
	{
	case WM_LBUTTONDOWN:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		OnLButtonDown(p);
		break;
		}
	case WM_LBUTTONUP:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		OnLButtonUp(p);
		break;
		}
	case WM_MOUSEMOVE:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		OnPointerMoved(p);
		break;
		}
	default:
		{
		bhandled=false;
		break;
		}
	}
}

VOID Splitter::OnLButtonDown(POINT const& pt)
{
Handle<CoreControl> hctl0;
Handle<CoreControl> hctl1;
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	if(!hctl0)
		{
		hctl0=hchild;
		continue;
		}
	hctl1=hchild;
	break;
	}
if(!hctl0||!hctl1)
	return;
RECT const& rc0=hctl0->GetRect();
if(cOrientation==SplitterOrientation::Horizontal)
	{
	iStartSize=rc0.Bottom-rc0.Top;
	}
else
	{
	iStartSize=rc0.Right-rc0.Left;
	}
iCurrentSize=iStartSize;
GetCursorPos(&ptStart);
SetCapture(hWindow);
}

VOID Splitter::OnLButtonUp(POINT const& pt)
{
if(!iStartSize)
	return;
ReleaseCapture();
iStartSize=0;
}

VOID Splitter::OnPointerMoved(POINT const& pt)
{
if(!iStartSize)
	return;
RECT const& rc=GetRect();
::POINT p;
GetCursorPos(&p);
if(cOrientation==SplitterOrientation::Horizontal)
	{
	INT dy=p.y-ptStart.y;
	INT iheight=MAX(iStartSize+dy, iMinSize0);
	if(iMaxSize0)
		iheight=MIN(iheight, iMaxSize0);
	INT imax=rc.Right-rc.Left-iMinSize1-iSize;
	iCurrentSize=MIN(iheight, imax);
	}
else
	{
	INT dx=p.x-ptStart.x;
	INT iwidth=MAX(iStartSize+dx, iMinSize0);
	if(iMaxSize0)
		iwidth=MIN(iwidth, iMaxSize0);
	INT imax=rc.Bottom-rc.Top-iMinSize1-iSize;
	iCurrentSize=MIN(iwidth, imax);
	}
Invalidate();
InvalidateArrangement();
}

}}
