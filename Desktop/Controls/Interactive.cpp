//=================
// Interactive.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include <windowsx.h>
#include "Container.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Interactive::Interactive(Handle<Container> hparent):
CoreWindow(hparent),
CoreControl(hparent),
CoreInteractive(hparent),
Control(hparent),
TabStop(this, false)
{
Created.Add(this, &Interactive::OnCreated);
Focused.Add(this, &Interactive::OnFocused);
MessageReceived.Add(this, &Interactive::OnMessageReceived);
}


//================
// Common Private
//================

VOID Interactive::OnCreated(Handle<DesktopWindow> hsender)
{
BOOL btabstop=GetStyle(WS_TABSTOP);
TabStop.SetInternal(btabstop);
TabStop.Changed.Add(this, &Interactive::OnTabStopChanged);
}

VOID Interactive::OnFocused(Handle<CoreInteractive> hsender)
{
for(auto pparent=pParent; pparent!=nullptr; pparent=pparent->GetParent())
	{
	auto pclass=(LPCSTR)GetProp(pparent->GetHandle(), "WindowClass");
	if(!pclass)
		continue;
	if(StringCompare(pclass, "Dialog")==0)
		{
		SendMessage(pparent->GetHandle(), WM_NEXTDLGCTL, (WPARAM)hWindow, true);
		return;
		}
	}
if(hWindow)
	::SetFocus(hWindow);
}

VOID Interactive::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(bhandled)
	return;
bhandled=true;
switch(umsg)
	{
	case WM_CONTEXTMENU:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		ContextMenu(this, p);
		break;
		}
	case WM_KEYDOWN:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		KeyDown(this, key);
		break;
		}
	case WM_KEYUP:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		KeyUp(this, key);
		break;
		}
	/*case WM_KILLFOCUS:
		{
		HWND hwndf=(HWND)wparam;
		Handle<Window> hfocus=(Window*)GetProp(hwndf, "WindowHandle");
		FocusLost(this);
		break;
		}*/
	case WM_LBUTTONDBLCLK:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		DoubleClicked(this, p);
		break;
		}
	case WM_LBUTTONDOWN:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		LButtonDown(this, p);
		break;
		}
	case WM_LBUTTONUP:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		LButtonUp(this, p);
		break;
		}
	case WM_MOUSEMOVE:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		PointerMoved(this, p);
		break;
		}
	case WM_MOUSEWHEEL:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		INT idelta=GET_WHEEL_DELTA_WPARAM(wparam);
		PointerWheel(this, p, idelta);
		break;
		}
	case WM_RBUTTONDOWN:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		RButtonDown(this, p);
		break;
		}
	case WM_RBUTTONUP:
		{
		POINT p;
		p.Left=GET_X_LPARAM(lparam);
		p.Top=GET_Y_LPARAM(lparam);
		RButtonUp(this, p);
		break;
		}
	case WM_SETFOCUS:
		{
		SetFocus();
		/*HWND hwndoldf=(HWND)wparam;
		Handle<Window> holdfocus=(Window*)GetProp(hwndoldf, "WindowHandle");
		FocusSet(this, holdfocus);*/
		break;
		}
	default:
		{
		bhandled=false;
		break;
		}
	}
}

VOID Interactive::OnTabStopChanged(BOOL btabstop)
{
SetStyle(WS_TABSTOP, btabstop);
}

}}