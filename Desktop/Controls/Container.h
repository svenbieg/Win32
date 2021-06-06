//=============
// Container.h
//=============

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Container.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//===========
// Container
//===========

class Container: public Control, public virtual Core::Controls::Container
{
public:
	// Con-/Destructors
	Container();
	Container(Handle<Container> Parent, WindowInfo* Info=nullptr);

	// Common
	Event<Window, Handle<Control>, UINT, BOOL&> Command;
	static Container* Current;
	Event<Window, DRAWITEMSTRUCT const*> ItemDrawed;
	Event<Window, NMHDR const*, BOOL&, LRESULT&> Notified;
	VOID Repaint()override;

private:
	// Common
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
};

}}