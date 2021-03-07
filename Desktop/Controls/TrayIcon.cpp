//==============
// TrayIcon.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include <CommCtrl.h>
#include <shellapi.h>
#include "Desktop/Application.h"
#include "Desktop/AppMessages.h"
#include "Desktop/AppWindow.h"
#include "Storage/ProcessBuffer.h"
#include "Process.h"
#include "TrayIcon.h"

using namespace Storage;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TrayIcon::TrayIcon(Handle<Application> happ, Handle<AppWindow> happwnd, UINT uico):
Icon(this),
Menu(this),
Text(this),
hApplication(happ),
hAppWindow(happwnd),
iShowCmd(SW_RESTORE)
{
Icon=new Graphics::Gdi::Icon(uico, GetSystemMetrics(SM_CXSMICON));
Icon.Changed.Add(this, &TrayIcon::OnIconChanged);
Text.Changed.Add(this, &TrayIcon::OnTextChanged);
hApplication->UnhandledException.Add(this, &TrayIcon::OnApplicationUnhandledException);
hAppWindow->MessageReceived.Add(this, &TrayIcon::OnAppWindowMessageReceived);
NOTIFYICONDATA nid;
ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
nid.cbSize=sizeof(NOTIFYICONDATA);
nid.uVersion=NOTIFYICON_VERSION_4;
nid.hWnd=hAppWindow->GetHandle();
nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
nid.hIcon=Icon->GetHandle();
nid.uCallbackMessage=WM_TRAYICON;
if(!Shell_NotifyIcon(NIM_ADD, &nid))
	{
	DebugPrint("Shell_NotifyIcon() failed\n");
	return;
	}
}

TrayIcon::~TrayIcon()
{
hApplication->UnhandledException.Remove(this);
hAppWindow->MessageReceived.Remove(this);
}


//========
// Common
//========

VOID TrayIcon::Destroy()
{
if(!hApplication)
	return;
Icon=nullptr;
Menu=nullptr;
Text=nullptr;
hApplication->UnhandledException.Remove(this);
hAppWindow->MessageReceived.Remove(this);
hApplication=nullptr;
hAppWindow=nullptr;
}


//================
// Common Private
//================

RECT TrayIcon::GetRect()
{
RECT rc;
ZeroMemory(&rc, sizeof(RECT));
if(!hApplication)
	return rc;
HWND hwndtray=FindWindowExW(0, 0, L"Shell_TrayWnd", 0);
ASSERT(hwndtray);
if(!hwndtray)
	return rc;
HWND hwndtb=FindWindowExW(hwndtray, 0, L"ToolbarWindow32", 0);
ASSERT(hwndtb);
if(!hwndtb)
	return rc;
UINT ubtncount=(UINT)SendMessage(hwndtb, TB_BUTTONCOUNT, 0, 0);
if(!ubtncount)
	return rc;
DWORD dwprocid=0;
GetWindowThreadProcessId(hwndtb, &dwprocid);
Handle<Process> hproc=new Process(dwprocid, PROCESS_ALL_ACCESS);
Handle<ProcessBuffer> hbuf=new ProcessBuffer(hproc->GetHandle(), sizeof(TBBUTTON));
HWND happwnd=hAppWindow->GetHandle();
for(UINT ubtn=0; ubtn<ubtncount; ubtn++)
	{
	SendMessage(hwndtb, TB_GETBUTTON, ubtn, (LPARAM)hbuf->Begin());
	TBBUTTON tbb;
	if(!hproc->Read(hbuf->Begin(), sizeof(TBBUTTON), (VOID*)&tbb))
		break;
	HWND hwndowner=0;
	hproc->Read((VOID*)tbb.dwData, sizeof(HWND), (VOID*)&hwndowner);
	if(hwndowner!=happwnd)
		continue;
	if(tbb.fsState&TBSTATE_HIDDEN)
		return rc;
	SendMessage(hwndtb, TB_GETITEMRECT, ubtn, (LPARAM)hbuf->Begin());
	::RECT rcb;
	hproc->Read(hbuf->Begin(), sizeof(::RECT), (VOID*)&rcb);
	MapWindowPoints(hwndtb, 0, (POINT*)&rcb, 2);
	rc=RECT({ rcb.left, rcb.top, rcb.right, rcb.left });
	}
return rc;
}

VOID TrayIcon::OnApplicationUnhandledException(Handle<Application> happ, EXCEPTION_POINTERS* pptrs)
{
Icon=nullptr;
}

VOID TrayIcon::OnAppWindowMessageReceived(Handle<Window> hsender, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& status)
{
switch(umsg)
	{
	case WM_DESTROY:
		{
		Icon=nullptr;
		break;
		}
	case WM_SIZE:
		{
		if(wparam==SIZE_MINIMIZED)
			{
			WINDOWPLACEMENT wp;
			GetWindowPlacement(hAppWindow->GetHandle(), &wp);
			iShowCmd=wp.showCmd;
			if(iShowCmd==SW_MINIMIZE||iShowCmd==SW_SHOWMINIMIZED)
				iShowCmd=SW_RESTORE;
			hAppWindow->Visible=false;
			bhandled=true;
			}
		break;
		}
	case WM_TRAYICON:
		{
		UINT ucode=LOWORD(lparam);
		switch(ucode)
			{
			case WM_LBUTTONUP:
				{
				OnLButtonUp(this);
				break;
				}
			case WM_RBUTTONUP:
				{
				OnRButtonUp(this);
				break;
				}
			}
		bhandled=true;
		break;
		}
	}
}

VOID TrayIcon::OnIconChanged(Handle<Graphics::Gdi::Icon> hico)
{
NOTIFYICONDATA nid;
ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
nid.cbSize=sizeof(NOTIFYICONDATA);
nid.uVersion=NOTIFYICON_VERSION_4;
nid.hWnd=hAppWindow->GetHandle();
if(hico)
	{
	nid.uFlags=NIF_ICON;
	nid.hIcon=hico->GetHandle();
	Shell_NotifyIcon(NIM_MODIFY, &nid);
	}
else
	{
	Shell_NotifyIcon(NIM_DELETE, &nid);
	}
}

VOID TrayIcon::OnLButtonUp(Handle<TrayIcon> htrayicon)
{
if(!hAppWindow)
	return;
HWND happwnd=hAppWindow->GetHandle();
if(!hAppWindow->Visible)
	{
	hAppWindow->Show(iShowCmd);
	SetForegroundWindow(happwnd);
	return;
	}
WINDOWPLACEMENT wp;
GetWindowPlacement(happwnd, &wp);
iShowCmd=wp.showCmd;
if(iShowCmd==SW_MINIMIZE||iShowCmd==SW_SHOWMINIMIZED)
	iShowCmd=SW_RESTORE;
hAppWindow->Visible=false;
}

VOID TrayIcon::OnRButtonUp(Handle<TrayIcon> htrayicon)
{
if(!hAppWindow)
	return;
if(Menu)
	{
	POINT p;
	GetCursorPos(&p);
	HWND happwnd=hAppWindow->GetHandle();
	SetForegroundWindow(happwnd);
	Menu->Show(p.x, p.y);
	PostMessageW(happwnd, WM_NULL, 0, 0);
	}
}

VOID TrayIcon::OnTextChanged(Handle<String> htext)
{
if(!hAppWindow)
	return;
NOTIFYICONDATA nid;
ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
nid.cbSize=sizeof(NOTIFYICONDATA);
nid.uVersion=NOTIFYICON_VERSION_4;
nid.hWnd=hAppWindow->GetHandle();
nid.uFlags=NIF_TIP;
if(htext)
	StringCopy(nid.szTip, 128, htext->Begin());
Shell_NotifyIcon(NIM_MODIFY, &nid);
}

}}
