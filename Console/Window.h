//==========
// Window.h
//==========

#pragma once


//=======
// Using
//=======

#include "Desktop/Window.h"


//===========
// Namespace
//===========

namespace Console {


//========
// Window
//========

class Window: public Desktop::Window
{
public:
	// Con-/Destructors
	Window();

	// Common
	static Handle<Window> Current;
};

}