//============
// MenuItem.h
//============

#pragma once


//=======
// Using
//=======

#include "Graphics/Gdi/Icon.h"
#include "Graphics/Gdi/Bitmap.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class Menu;
class PopupMenu;


//===========
// Menu-Item
//===========

class MenuItem: public Object
{
private:
	// Using
	using Bitmap=Graphics::Gdi::Bitmap;

public:
	// Con-/Destructors
	MenuItem(Handle<Menu> Parent, INT Position=-1, WORD Id=0, Handle<String> Text=nullptr, WORD Icon=0);

	// Common
	Handle<MenuItem> AddItem(WORD Id=0, Handle<String> Text=nullptr, WORD Icon=0) { return InsertItem(-1, Id, Text, Icon); }
	Event<MenuItem> Changed;
	Property<MenuItem, BOOL> Checked;
	Property<MenuItem, BOOL> Enabled;
	WORD GetId()const { return uId; }
	VariableHandle<MenuItem, Graphics::Gdi::Icon> Icon;
	Handle<MenuItem> InsertItem(INT Position, WORD Id=0, Handle<String> Text=nullptr, WORD Icon=0);
	Handle<Menu> Parent;
	VOID SetIcon(WORD Id);
	VariableHandle<MenuItem, PopupMenu> SubMenu;
	VariableHandle<MenuItem, String> Text;

private:
	// Common
	VOID OnCheckedChanged(BOOL Checked);
	VOID OnCheckedReading(BOOL& Checked);
	VOID OnEnabledChanged(BOOL Enabled);
	VOID OnEnabledReading(BOOL& Enabled);
	VOID OnIconChanged(Handle<Graphics::Gdi::Icon> Icon);
	VOID OnSubMenuChanged(Handle<PopupMenu> SubMenu);
	VOID OnTextChanged(Handle<String> Text);
	Handle<Bitmap> hBitmap;
	WORD uId;
};

}}}
