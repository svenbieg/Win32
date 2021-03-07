//============
// MainMenu.h
//============

#pragma once


//=======
// Using
//=======

#include "Menu.h"


//======================
// Forward-Declarations
//======================

namespace Desktop
{
class AppWindow;
}


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//===========
// Main-Menu
//===========

class MainMenu: public Menu
{
public:
	// Con-/Destructors
	MainMenu(Handle<AppWindow> Parent);

private:
	// Common
	VOID OnChanged(Handle<Menu> Menu);
};

}}}
