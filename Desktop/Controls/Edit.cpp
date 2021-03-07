//==========
// Edit.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Edit.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Edit::Edit(Handle<Container> hparent, Handle<String> htext):
CoreWindow(hparent),
CoreControl(hparent),
CoreInteractive(hparent),
Interactive(hparent),
ReadOnly(this, false),
hEdit(NULL),
pOldWndProc(nullptr)
{
MessageReceived.Add(this, &Edit::OnMessageReceived);
Padding={ 2, 2, 0, 2 };
ReadOnly.Changed.Add(this, &Edit::OnReadOnlyChanged);
TabStop.Changed.Add(this, &Edit::OnTabStopChanged);
WindowInfo info;
info.BackgroundBrush=new GdiBrush(GetSysColor(COLOR_WINDOW));
info.Class="EditHelper";
info.Style=WS_VISIBLE;
info.StyleEx=WS_EX_CLIENTEDGE|WS_EX_CONTROLPARENT;
Create(info);
LPCSTR ptext=htext? htext->Begin(): nullptr;
HINSTANCE hinst=GetModuleHandle(nullptr);
hEdit=CreateWindow("EDIT", ptext, ES_AUTOHSCROLL|WS_TABSTOP|WS_VISIBLE, 0, 0, 1, 1, hWindow, NULL, hinst, this);
SetProp(hEdit, "WindowHandle", (HANDLE)this);
pOldWndProc=SetWindowProc(EditWndProc);
}

Edit::~Edit()
{
DestroyWindow(hEdit);
}


//========
// Common
//========

UINT Edit::GetLimit()
{
return (UINT)SendMessage(hEdit, EM_GETLIMITTEXT, 0, 0);
}

Handle<String> Edit::GetText()
{
UINT ulen=SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
Handle<String> htext=new String(ulen+1, nullptr);
SendMessage(hEdit, WM_GETTEXT, ulen+1, (LPARAM)htext->Begin());
return htext;
}

VOID Edit::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Interactive::Rearrange(hdc, rc);
MoveWindow(hEdit, rc.Left, rc.Top, rc.Right-rc.Left, rc.Bottom-rc.Top, true);
}

VOID Edit::SetLimit(UINT ulimit)
{
SendMessage(hEdit, EM_LIMITTEXT, ulimit, 0);
}

VOID Edit::SetText(Handle<String> htext)
{
SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)htext->Begin());
}


//==================
// Common Protected
//==================

Graphics::SIZE Edit::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
minsize.Height+=Font->GetHeight();
return minsize;
}


//================
// Common Private
//================

LRESULT CALLBACK Edit::EditWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
Handle<Edit> hedit=(Edit*)GetProp(hwnd, "WindowHandle");
if(!hedit)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
switch(umsg)
	{
	case WM_SETFOCUS:
		{
		hedit->SetFocus();
		return 0;
		}
	}
return CallWindowProc(hedit->pOldWndProc, hwnd, umsg, wparam, lparam);
}

VOID Edit::OnMessageReceived(Handle<Window> hsender, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(umsg==WM_COMMAND)
	{
	switch(HIWORD(wparam))
		{
		case EN_CHANGE:
			{
			TextChanged(this);
			break;
			}
		}
	}
}

VOID Edit::OnReadOnlyChanged(BOOL breadonly)
{
UINT ucolor=breadonly? COLOR_BTNFACE: COLOR_WINDOW;
BackgroundBrush=new GdiBrush(GetSysColor(ucolor));
SendMessage(hEdit, EM_SETREADONLY, breadonly, 0);
}

VOID Edit::OnTabStopChanged(BOOL btabstop)
{
SetStyle(WS_TABSTOP, false);
UINT ustyle=GetWindowLong(hEdit, GWL_STYLE);
SetFlag(ustyle, WS_TABSTOP, btabstop);
SetWindowLong(hEdit, GWL_STYLE, ustyle);
}

}}