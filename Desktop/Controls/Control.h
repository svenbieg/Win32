//===========
// Control.h
//===========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Control.h"
#include "Desktop/Window.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//======================
// Forward-Declarations
//======================

class Container;


//=========
// Control
//=========

class Control: public Window
{
public:
	// Con-/Destructors
	Control(Handle<Container> Parent, WindowInfo* Info=nullptr);
};

}}
