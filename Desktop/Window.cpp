//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "uxtheme")

#include <Uxtheme.h>
#include <windowsx.h>
#include "Controls/Container.h"

using namespace Desktop::Controls;


//===========
// Namespace
//===========

namespace Desktop {


//========
// Common
//========

VOID Window::Destroy()
{
if(hWindow)
	DestroyWindow(hWindow);
}

Handle<Container> Window::GetParent()const
{
return pParent;
}

VOID* Window::GetProperty(Handle<String> hname)
{
if(!hname||!hWindow)
	return nullptr;
return ::GetProp(hWindow, hname->Begin());
}

UINT Window::GetStyle()
{
if(!hWindow)
	return 0;
return GetWindowLong(hWindow, GWL_STYLE);
}

BOOL Window::GetStyle(UINT uflag)
{
if(!hWindow)
	return false;
return (GetWindowLong(hWindow, GWL_STYLE)&uflag)!=0;
}

UINT Window::GetStyleEx()
{
if(!hWindow)
	return 0;
return GetWindowLong(hWindow, GWL_EXSTYLE);
}

VOID Window::Invalidate()
{
if(hWindow)
	InvalidateRect(hWindow, nullptr, true);
}

VOID Window::Repaint()
{
if(!hWindow)
	return;
InvalidateRect(hWindow, nullptr, true);
SendMessage(hWindow, WM_NCPAINT, 1, 0);
UpdateWindow(hWindow);
}

VOID Window::SetProperty(Handle<String> hname, VOID* pvalue)
{
if(!hname||!hWindow)
	return;
::SetProp(hWindow, hname->Begin(), pvalue);
}

VOID Window::SetStyle(UINT ustyle)
{
if(!hWindow)
	return;
UINT uoldstyle=GetWindowLong(hWindow, GWL_STYLE);
SetWindowLong(hWindow, GWL_STYLE, ustyle);
Visible=ustyle&WS_VISIBLE;
if((ustyle&WS_OVERLAPPEDWINDOW)!=(uoldstyle&WS_OVERLAPPEDWINDOW))
	SetWindowPos(hWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
}

VOID Window::SetStyle(UINT uflag, BOOL bset)
{
if(!hWindow)
	return;
UINT ustyle=GetWindowLong(hWindow, GWL_STYLE);
SetFlag(ustyle, uflag, bset);
SetStyle(ustyle);
}

VOID Window::SetStyleEx(UINT ustyle)
{
if(!hWindow)
	return;
UINT uoldstyle=GetWindowLong(hWindow, GWL_EXSTYLE);
SetWindowLongW(hWindow, GWL_EXSTYLE, ustyle);
UINT uframestyles=WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_WINDOWEDGE;
if((ustyle&uframestyles)!=(uoldstyle&uframestyles))
	SetWindowPos(hWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
}

WNDPROC Window::SetWindowProc(WNDPROC pproc)
{
return (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)pproc);
}


//============================
// Con-/Destructors Protected
//============================

Window::Window(Handle<Container> hparent):
Core::Controls::Control(hparent),
// Common
BackgroundBrush(this),
// Common Protected
hWindow(NULL),
pParent(hparent),
// Common Private
hThis(this),
pOldWndProc(nullptr),
uDotsPerInch(96)
{
MessageReceived.Add(this, &Window::OnMessageReceived);
Moved.Add(this, &Window::OnMoved);
Visible.Changed.Add(this, &Window::OnVisibleChanged);
}


//==================
// Common Protected
//==================

VOID Window::Create(WindowInfo const& info)
{
UINT ustyle=info.Style;
BackgroundBrush=info.BackgroundBrush;
Cursor=info.Cursor;
Font=info.Font;
HWND hparent=NULL;
if(pParent)
	{
	hparent=pParent->GetHandle();
	ustyle|=WS_CHILD;
	if(!BackgroundBrush)
		BackgroundBrush=pParent->BackgroundBrush;
	if(!Cursor)
		Cursor=pParent->Cursor;
	if(!Font)
		Font=pParent->Font;
	}
if(!BackgroundBrush)
	BackgroundBrush=new GdiBrush(GetSysColor(COLOR_BTNFACE));
if(!Cursor)
	Cursor=new Graphics::Gdi::Cursor();
if(!Font)
	Font=new Graphics::Gdi::Font();
BOOL bvisible=info.Style&WS_VISIBLE;
Visible.SetInternal(bvisible);
HINSTANCE hinst=GetModuleHandle(nullptr);
UINT uwidth=MAX(info.Width, 1);
UINT uheight=MAX(info.Height, 1);
WNDCLASSEX wc;
ZeroMemory(&wc, sizeof(WNDCLASSEX));
wc.cbSize=sizeof(WNDCLASSEX);
BOOL bpredef=GetClassInfoEx(hinst, info.Class, &wc);
if(bpredef)
	{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(&WndProc, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VOID* p=mbi.AllocationBase;
	VirtualQuery(wc.lpfnWndProc, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	if(p==mbi.AllocationBase)
		bpredef=false;
	}
if(!bpredef)
	{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.hCursor=Cursor->GetHandle();
	if(info.Icon)
		{
		UINT usize=GetSystemMetrics(SM_CXICON);
		wc.hIcon=(HICON)LoadImage(hinst, MAKEINTRESOURCE(info.Icon), IMAGE_ICON, usize, usize, 0);
		}
	if(info.IconSm)
		{
		UINT usize=GetSystemMetrics(SM_CXSMICON);
		wc.hIconSm=(HICON)LoadImage(hinst, MAKEINTRESOURCE(info.IconSm), IMAGE_ICON, usize, usize, 0);
		}
	wc.hInstance=hinst;
	wc.lpfnWndProc=&WndProc;
	wc.lpszClassName=info.Class;
	wc.style=info.ClassStyle;
	SetLastError(0);
	RegisterClassEx(&wc);
	DWORD dwerr=GetLastError();
	switch(dwerr)
		{
		case ERROR_SUCCESS:
			break;
		case ERROR_CLASS_ALREADY_EXISTS:
			break;
		default:
			{
			DebugPrint("RegisterClass failed\n");
			return;
			}
		}
	}
SetLastError(0);
LPCTSTR ptext=info.Text? info.Text->Begin(): nullptr;
hWindow=CreateWindowEx(info.StyleEx, info.Class, ptext, ustyle, info.Left, info.Top, uwidth, uheight, hparent, (HMENU)info.Id, hinst, this);
if(hWindow==INVALID_HANDLE_VALUE)
	hWindow=NULL;
if(hWindow==NULL)
	{
	DebugPrint("CreateWindow failed\n");
	return;
	}
if(bpredef)
	{
	SetProp(hWindow, "WindowHandle", (HANDLE)this);
	pOldWndProc=SetWindowProc(ClassWndProc);
	Handle<GdiFont> hfont=Font.As<GdiFont>();
	if(hfont)
		SendMessage(hWindow, WM_SETFONT, (WPARAM)hfont->GetHandle(), false);
	}
BackgroundBrush.Changed.Add(this, &Window::OnBackgroundBrushChanged);
Visible.Changed.Add(this, &Window::OnVisibleChanged);
}


//================
// Common Private
//================

LRESULT CALLBACK Window::ClassWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
Handle<Window> hwindow=(Window*)GetProp(hwnd, "WindowHandle");
if(!hwindow)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
BOOL bhandled=false;
LRESULT status;
hwindow->MessageReceived(hwindow, umsg, wparam, lparam, bhandled, status);
if(bhandled)
	return status;
return CallWindowProc(hwindow->pOldWndProc, hwnd, umsg, wparam, lparam);
}

VOID Window::OnBackgroundBrushChanged(Handle<GdiBrush> hbr)
{
Invalidate();
}

VOID Window::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(bhandled)
	return;
bhandled=true;
switch(umsg)
	{
	case WM_CREATE:
		{
		Created(this);
		break;
		}
	case WM_DESTROY:
		{
		Destroyed(this);
		hThis=nullptr;
		break;
		}
	case WM_ERASEBKGND:
		{
		lr=1;
		if(!BackgroundBrush)
			break;
		if(!BackgroundBrush->GetColor().GetAlpha())
			break;
		HDC hdc=(HDC)wparam;
		::RECT rc;
		::GetClientRect(hWindow, &rc);
		FillRect(hdc, &rc, BackgroundBrush->GetHandle());
		break;
		}
	case WM_PAINT:
		{
		if(!Painted)
			{
			bhandled=false;
			break;
			}
		PAINTSTRUCT ps;
		HDC hdc=BeginPaint(hWindow, &ps);
		::RECT rc;
		::GetClientRect(hWindow, &rc);
		Handle<GdiContext> hgdi=new GdiContext(hdc);
		hgdi->Clip={ 0, 0, rc.right, rc.bottom };
		hgdi->Offset={ 0, 0 };
		Painted(this, hgdi);
		EndPaint(hWindow, &ps);
		break;
		}
	case WM_SHOWWINDOW:
		{
		BOOL bvisible=(BOOL)wparam;
		Visible.SetInternal(bvisible);
		bhandled=false;
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

VOID Window::OnMoved(RECT const& rc)
{
if(hWindow)
	MoveWindow(hWindow, rc.Left, rc.Top, rc.Right-rc.Left, rc.Bottom-rc.Top, false);
}

VOID Window::OnVisibleChanged(BOOL bvisible)
{
if(!hWindow)
	return;
ShowWindow(hWindow, bvisible? SW_SHOW: SW_HIDE);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
Handle<Window> hwindow=(Window*)GetProp(hwnd, "WindowHandle");
if(umsg==WM_CREATE)
	{
	CREATESTRUCT* pcs=(CREATESTRUCT*)lparam;
	SetProp(hwnd, "WindowHandle", pcs->lpCreateParams);
	hwindow=(Window*)pcs->lpCreateParams;
	hwindow->hWindow=hwnd;
	Handle<GdiFont> hfont=hwindow->Font.As<GdiFont>();
	if(hfont)
		SetWindowFont(hwnd, hfont->GetHandle(), true);
	}
if(!hwindow)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
BOOL bhandled=false;
LRESULT status=0;
hwindow->MessageReceived(hwindow, umsg, wparam, lparam, bhandled, status);
if(bhandled)
	return status;
if(umsg==WM_CREATE)
	return 0;
return DefWindowProc(hwnd, umsg, wparam, lparam);
}

}