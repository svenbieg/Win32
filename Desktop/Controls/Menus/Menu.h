//========
// Menu.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Desktop/Window.h"
#include "MenuItem.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//======
// Menu
//======

class Menu: public Object
{
protected:
	// Using
	using ItemList=Collections::Cluster<Handle<MenuItem>>;

public:
	// Common
	Handle<MenuItem> AddItem(WORD Id=0, Handle<String> Text=nullptr, WORD Icon=0) { return InsertItem(-1, Id, Text, Icon); }
	Event<Menu> Changed;
	VOID CheckItem(WORD Id, BOOL Checked);
	VOID Clear();
	VOID Destroy();
	VOID EnableItem(WORD Id, BOOL Enabled);
	Handle<MenuItem> GetItem(WORD Id);
	HMENU GetHandle() { return hMenu; }
	UINT GetItemCount();
	Handle<MenuItem> InsertItem(INT Position, WORD Id=0, Handle<String> Text=nullptr, WORD Icon=0);
	Handle<ItemList> Items;
	VOID Notify() { Changed(this); }
	Event<Menu, Handle<MenuItem>> SelectionChanged;
	VOID SetItemIcon(WORD Id, WORD Icon);
	VOID SetItemText(WORD Id, Handle<String> Text);
	Handle<Window> Parent;

protected:
	// Con-/Destructors
	Menu(Handle<Window> Parent);

	// Common
	HMENU hMenu;

private:
	// Common
	VOID OnParentMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Status);
};

}}}
