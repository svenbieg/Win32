//==============
// Overlapped.h
//==============

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Control.h"
#include "Desktop/Controls/Container.h"


//===========
// Namespace
//===========

namespace Desktop {


//============
// Overlapped
//============

class Overlapped: public Desktop::Controls::Container
{
public:
	// Con-/Destructors
	Overlapped(WindowInfo* Info=nullptr);

	// Common
	VOID Activate();
	Event<Window> Activated;
	Event<Window, HDC> BorderPainted;
	VOID BringToFront();
	Event<Window> Closed;
	RECT GetBorderWidth();
	SIZE GetMinSize();
	Event<Window> Maximized;
	Event<Window> Minimized;
	VOID MinimizeSize();
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect);

private:
	// Common
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID UpdateWindowRect();
};

}