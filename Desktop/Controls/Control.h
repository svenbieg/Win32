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

class Control: public Window, public virtual Core::Controls::Control
{
public:
	// Using
	using DesktopControl=Control;

	// Con-/Destructors
	Control(Handle<Container> Parent, WindowInfo* Info=nullptr);

	// Common
	VOID Create(WindowInfo const& Info) { DesktopWindow::Create(Info); }
};

}}
