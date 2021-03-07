//===========
// Mouse.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Mouse.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Input {


//==================
// Con-/Destructors
//==================

Mouse::Mouse():
hHook(NULL),
uFlags(MouseFlags::None)
{
HINSTANCE hinst=GetModuleHandle(nullptr);
DWORD dwthread=GetCurrentThreadId();
SetLastError(0);
hHook=SetWindowsHookEx(WH_MOUSE, MouseProc, hinst, dwthread);
if(hHook==INVALID_HANDLE_VALUE)
	hHook=NULL;
ASSERT(hHook);
}

Mouse::~Mouse()
{
if(hHook)
	UnhookWindowsHookEx(hHook);
}


//================
// Common Private
//================

LRESULT Mouse::MouseProc(INT icode, WPARAM wparam, LPARAM lparam)
{
throw E_NOTIMPL;
/*if(icode==HC_ACTION)
	{
	auto pinfo=(KBDLLHOOKSTRUCT*)lparam;
	BYTE ukey=(BYTE)pinfo->vkCode;
	if(wparam==WM_KEYDOWN)
		{
		OnKeyDown(ukey);
		}
	else if(wparam==WM_KEYUP)
		{
		OnKeyUp(ukey);
		}
	}*/
return CallNextHookEx(NULL, icode, wparam, lparam);
}

}}
