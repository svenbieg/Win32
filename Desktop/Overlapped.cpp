//================
// Overlapped.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "Controls/Container.h"
#include "Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

Overlapped::Overlapped(WindowInfo* pinfo):
CoreWindow(nullptr),
CoreControl(nullptr),
CoreContainer(nullptr),
Container(nullptr, pinfo)
{
MessageReceived.Add(this, &Overlapped::OnMessageReceived);
}


//========
// Common
//========

VOID Overlapped::Activate()
{
if(hWindow)
	SendMessage(hWindow, WM_ACTIVATE, WA_ACTIVE, 0);
}

VOID Overlapped::BringToFront()
{
if(hWindow)
	SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
}

Graphics::RECT Overlapped::GetBorderWidth()
{
if(!hWindow)
	return { 0, 0, 0, 0 };
::RECT rcc;
GetClientRect(hWindow, &rcc);
::RECT rc(rcc);
AdjustWindowRectEx(&rc, GetStyle(), GetMenu(hWindow)!=NULL, GetStyleEx());
RECT rcborder;
rcborder.Left=rcc.left-rc.left;
rcborder.Right=rc.right-rcc.right;
rcborder.Top=rcc.top-rc.top;
rcborder.Bottom=rc.bottom-rcc.bottom;
return rcborder;
}

Graphics::SIZE Overlapped::GetMinSize()
{
RECT rcb=GetBorderWidth();
SIZE minsize(rcb.Left+rcb.Right, rcb.Bottom+rcb.Top);
auto hcontent=Children->GetAt(0);
if(hcontent)
	{
	Handle<GdiContext> hdc=new GdiContext();
	SIZE msc=hcontent->GetMinSize(hdc);
	minsize.Width+=msc.Width;
	minsize.Height+=msc.Height;
	}
return minsize;
}

VOID Overlapped::MinimizeSize()
{
RECT rc(rcRect);
SIZE minsize=GetMinSize();
rc.Right=rc.Left+minsize.Width;
rc.Bottom=rc.Top+minsize.Height;
Move(rc);
}

VOID Overlapped::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
auto hcontent=Children->GetAt(0);
if(!hcontent)
	return;
if(!hcontent->Visible.GetInternal())
	return;
hcontent->Move(rc);
}


//================
// Common Private
//================

VOID Overlapped::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(bhandled)
	{
	if(umsg==WM_SIZE)
		{
		if(wparam==SIZE_MINIMIZED)
			Minimized(this);
		if(wparam==SIZE_MAXIMIZED)
			Maximized(this);
		}
	return;
	}
bhandled=true;
switch(umsg)
	{
	case WM_ACTIVATE:
		{
		Activated(this);
		break;
		}
	case WM_CLOSE:
		{
		if(!Closed)
			{
			DestroyWindow(hWindow);
			break;
			}
		Closed(this);
		break;
		}
	case WM_CREATE:
		{
		UpdateWindowRect();
		break;
		}
	case WM_MOVE:
		{
		UpdateWindowRect();
		break;
		}
	case WM_NCPAINT:
		{
		if(!BorderPainted)
			{
			bhandled=false;
			break;
			}
		HDC hdc;
		hdc=GetDCEx(hWindow, (HRGN)wparam, DCX_WINDOW|DCX_INTERSECTRGN);
		BorderPainted(this, hdc);
		ReleaseDC(hWindow, hdc);
		break;
		}
	case WM_SHOWWINDOW:
		{
		BOOL bvisible=(BOOL)wparam;
		Visible.SetInternal(bvisible);
		bhandled=false;
		break;
		}
	case WM_SIZE:
		{
		UpdateWindowRect();
		Handle<GdiContext> hdc=new GdiContext();
		::RECT rc;
		GetClientRect(hWindow, &rc);
		RECT rcc({ 0, 0, rc.right, rc.bottom });
		Rearrange(hdc, rcc);
		Repaint();
		break;
		}
	case WM_SIZING:
		{
		lr=true;
		auto hcontent=Children->GetAt(0);
		if(!hcontent)
			return;
		auto prc=(::RECT*)lparam;
		SIZE minsize=GetMinSize();
		UINT uwidth=(UINT)(prc->right-prc->left);
		uwidth=MAX(uwidth, minsize.Width);
		UINT uheight=(UINT)(prc->bottom-prc->top);
		uheight=MAX(uheight, minsize.Height);
		if(wparam==WMSZ_RIGHT||wparam==WMSZ_TOPRIGHT||wparam==WMSZ_BOTTOMRIGHT)
			{
			prc->right=prc->left+uwidth;
			}
		else
			{
			prc->left=prc->right-uwidth;
			}
		if(wparam==WMSZ_BOTTOM||wparam==WMSZ_BOTTOMLEFT||wparam==WMSZ_BOTTOMRIGHT)
			{
			prc->bottom=prc->top+uheight;
			}
		else
			{
			prc->top=prc->bottom-uheight;
			}
		break;
		}
	case WM_TIMER:
		{
		TimerTriggered(this, LOWORD(wparam));
		break;
		}
	default:
		{
		bhandled=false;
		break;
		}
	}
}

VOID Overlapped::UpdateWindowRect()
{
if(!hWindow)
	return;
::RECT rc;
GetWindowRect(hWindow, &rc);
rcRect={ rc.left, rc.top, rc.right, rc.bottom };
}

}