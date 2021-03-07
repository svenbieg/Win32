//=============
// PopupMenu.h
//=============

#pragma once


//=======
// Using
//=======

#include "Menu.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//============
// Popup-Menu
//============

class PopupMenu: public Menu
{
public:
	// Con-/Destructors
	PopupMenu(Handle<Window> Window);
	
	// Common
	VOID Show(INT Left, INT Top);
	Event<PopupMenu> Showed;
};

}}}
