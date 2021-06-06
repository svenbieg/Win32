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
Desktop::Window(nullptr)
{
hWindow=GetConsoleWindow();
SetProperty("WindowHandle", this);
}


//========
// Common
//========

Handle<Window> Window::Current;

}