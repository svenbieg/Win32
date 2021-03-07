//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/Controls/Container.h"
#include "Window.h"


//===========
// Namespace
//===========

namespace Console {


//==================
// Con-/Destructors
//==================

Window::Window():
CoreWindow(nullptr),
DesktopWindow(nullptr)
{
hWindow=GetConsoleWindow();
SetProperty("WindowHandle", this);
Visible.SetInternal(true);
}


//========
// Common
//========

Handle<Window> Window::Current;

}