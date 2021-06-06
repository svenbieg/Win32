//============
// Dialog.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/Controls/Container.h"
#include "Desktop/AppMessages.h"
#include "Dialog.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

VOID Dialog::SetIcon(Handle<GdiIcon> hicon)
{
hIcon=hicon;
HICON hico=hIcon? hIcon->GetHandle(): NULL;
SendMessage(hWindow, WM_SETICON, ICON_SMALL, (LPARAM)hico);
}

INT_PTR Dialog::Show()
{
bModeless=false;
HINSTANCE hinst=GetModuleHandleW(NULL);
HWND hwndparent=pParent? pParent->GetHandle(): NULL;
INT_PTR iret=DialogBoxIndirectParam(hinst, (DLGTEMPLATE*)&cTemplate, hwndparent, DialogProc, (LPARAM)this);
return iret;
}

VOID Dialog::ShowModeless()
{
bModeless=true;
HINSTANCE hinst=GetModuleHandleW(NULL);
HWND hwndparent=pParent? pParent->GetHandle(): NULL;
CreateDialogIndirectParam(hinst, (DLGTEMPLATE*)&cTemplate, hwndparent, DialogProc, (LPARAM)this);
}


//============================
// Con-/Destructors Protected
//============================

Dialog::Dialog(Handle<Container> hparent, UINT ustyle, Handle<String> htitle, UINT uico):
Core::Controls::Container(hparent)
{
Closed.Add(this, &Dialog::OnClosed);
Destroyed.Add(this, &Dialog::OnDestroyed);
bModeless=false;
ZeroMemory(&cTemplate, sizeof(TEMPLATE));
cTemplate.dthHeader.style=ustyle|WS_CLIPCHILDREN|WS_VISIBLE;
StringCopy(cTemplate.pTitle, 80, htitle->Begin());
BackgroundBrush=new GdiBrush(GetSysColor(COLOR_BTNFACE));
Cursor=new GdiCursor(Cursors::Arrow);
if(pParent)
	Font=pParent->Font;
if(!Font)
	Font=new GdiFont(Fonts::MenuFont);
if(uico)
	hIcon=new GdiIcon(uico);
}


//================
// Common Private
//================

INT_PTR CALLBACK Dialog::DialogProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
Dialog* pdialog=(Dialog*)GetProp(hwnd, "WindowHandle");
switch(umsg)
	{
	case WM_CTLCOLORDLG:
		{
		if(pdialog->BackgroundBrush)
			return (INT_PTR)pdialog->BackgroundBrush->GetHandle();
		}
	case WM_INITDIALOG:
		{
		pdialog=(Dialog*)lparam;
		pdialog->hWindow=hwnd;
		pdialog->SetProperty("WindowClass", "Dialog");
		pdialog->SetProperty("WindowHandle", pdialog);
		Handle<GdiFont> hfont=pdialog->Font.As<GdiFont>();
		if(hfont)
			SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont->GetHandle(), false);
		if(pdialog->hIcon)
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)pdialog->hIcon->GetHandle());
		pdialog->Visible.SetInternal(true);
		pdialog->Initialized(pdialog);
		pdialog->Reposition();
		auto hparent=pdialog->GetParent();
		if(hparent)
			hparent->Visible.Changed.Add(pdialog, &Dialog::OnParentVisibleChanged);
		return true;
		}
	/*case WM_WINDOWPOSCHANGING:
		{
		if(!pdialog->Visible)
			{
			auto ppos=(WINDOWPOS*)lparam;
			ppos->flags&=~SWP_SHOWWINDOW;
			}
		SetWindowLongPtr(hwnd, DWLP_MSGRESULT, 0);
		return true;
		}*/
	}
BOOL bhandled=false;
LRESULT lr=0;
pdialog->MessageReceived(pdialog, umsg, wparam, lparam, bhandled, lr);
if(bhandled)
	{
	SetWindowLongPtr(hwnd, DWLP_MSGRESULT, lr);
	return true;
	}
return false;
}

VOID Dialog::OnClosed(Handle<Window> hwnd)
{
EndDialog(IDCLOSE);
}

VOID Dialog::OnDestroyed(Handle<Window> hwnd)
{
if(pParent)
	pParent->Visible.Changed.Remove(this);
}

VOID Dialog::OnParentVisibleChanged(BOOL bvisible)
{
Visible=bvisible;
}

VOID Dialog::Reposition()
{
SIZE minsize=GetMinSize();
HWND hwnddesktop=GetDesktopWindow();
::RECT rcdesktop;
GetWindowRect(hwnddesktop, &rcdesktop);
INT idesktopwidth=rcdesktop.right-rcdesktop.left;
INT idesktopheight=rcdesktop.bottom-rcdesktop.top;
RECT rcparent={ rcdesktop.left, rcdesktop.top, rcdesktop.right, rcdesktop.bottom };
if(pParent&&pParent->Visible)
	rcparent=pParent->GetRect();
RECT rc;
rc.Left=MAX((idesktopwidth-minsize.Width)/2, rcparent.Left);
rc.Top=MAX((idesktopheight-minsize.Height)/2-100, rcparent.Top);
rc.Right=rc.Left+minsize.Width;
rc.Bottom=rc.Top+minsize.Height;
if(rc.Right>rcparent.Right)
	{
	rc.Left=MAX(rcparent.Right-minsize.Width, 0);
	rc.Right=rc.Left+minsize.Width;
	}
if(rc.Bottom>rcparent.Bottom)
	{
	rc.Top=MAX(rcparent.Bottom-minsize.Height, 0);
	rc.Bottom=rc.Top+minsize.Height;
	}
Move(rc);
}

}}