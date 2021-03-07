//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Container.h"
#include "Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {


//============
// App-Window
//============

class AppWindow: public Overlapped
{
public:
	// Con-/Destructors
	AppWindow(Handle<String> Title=nullptr, INT Icon=0);

	// Common
	VOID Show(INT Command);
	VariableHandle<AppWindow, String> Title;

private:
	// Common
	VOID OnDestroyed(Handle<Window> Window);
	VOID OnTitleChanged(Handle<String> Title);
	Handle<GdiIcon> hIcon;
	Handle<GdiIcon> hIconSm;
};

}