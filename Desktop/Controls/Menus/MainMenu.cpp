//==============
// MainMenu.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/AppWindow.h"
#include "MainMenu.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

MainMenu::MainMenu(Handle<AppWindow> happwnd):
Menu(happwnd)
{
SetLastError(0);
hMenu=::CreateMenu();
if(hMenu==INVALID_HANDLE_VALUE)
	hMenu=NULL;
if(!hMenu)
	return;
Changed.Add(this, &MainMenu::OnChanged);
}


//================
// Common Private
//================

VOID MainMenu::OnChanged(Handle<Menu> hmenu)
{
SetMenu(Parent->GetHandle(), hMenu);
}

}}}
