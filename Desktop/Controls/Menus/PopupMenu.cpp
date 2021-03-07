//===============
// PopupMenu.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "PopupMenu.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

PopupMenu::PopupMenu(Handle<Window> hwnd):
Menu(hwnd)
{
SetLastError(0);
hMenu=CreatePopupMenu();
if(hMenu==INVALID_HANDLE_VALUE)
	hMenu=NULL;
ASSERT(hMenu);
}


//========
// Common
//========

VOID PopupMenu::Show(INT x, INT y)
{
Showed(this);
HWND hwnd=Parent? Parent->GetHandle(): NULL;
TrackPopupMenu(hMenu, 0, x, y, 0, hwnd, 0);
PostMessage(hwnd, WM_NULL, 0, 0);
}

}}}
