//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Console/Window.h"
#include "Devices/Serial/SerialPort.h"
#include "Devices/Clock.h"
#include "Application.h"

extern VOID Initialize();

using namespace Console;
using namespace Devices;
using namespace Devices::Serial;


//==========
// Callback
//==========

BOOL WINAPI ConsoleCallback(DWORD uid)
{
if(uid==CTRL_CLOSE_EVENT)
	{
	auto happ=Application::Current;
	return true;
	}
return false;
}


//=============
// Entry-Point
//=============

#ifndef _DESKTOP

INT main()
{
Clock::Current=new Clock();
Application::Version="1.0";
Window::Current=new Window();
Initialize();
auto happ=Application::Current;
if(!happ->Loop)
	return 0;
SetConsoleCtrlHandler(ConsoleCallback, true);
while(happ->Loop)
	{
	MSG msg;
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
		if(msg.message==WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	happ->Loop(happ);
	}
return 0;
}

#endif


//===========
// Namespace
//===========

namespace Console {


//==================
// Con-/Destructors
//==================

Application::Application(Handle<String> hname):
Core::Application(hname)
{}

}