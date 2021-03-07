//==============
// Keyboard.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Keyboard.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Input {


//==================
// Con-/Destructors
//==================

Keyboard::Keyboard():
hHook(NULL)
{
HINSTANCE hinst=GetModuleHandle(nullptr);
DWORD dwthread=GetCurrentThreadId();
SetLastError(0);
hHook=SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hinst, dwthread);
if(hHook==INVALID_HANDLE_VALUE)
	hHook=NULL;
ASSERT(hHook);
}

Keyboard::~Keyboard()
{
if(hHook)
	UnhookWindowsHookEx(hHook);
}


//========
// Common
//========

BOOL Keyboard::IsKeyDown(BYTE ukey)
{
throw E_NOTIMPL;
return GetKeyState(ukey)&0x8000;
}


//=================
// Desktop Private
//=================

LRESULT Keyboard::KeyboardProc(INT icode, WPARAM wparam, LPARAM lparam)
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
