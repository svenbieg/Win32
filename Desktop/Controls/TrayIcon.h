//============
// TrayIcon.h
//============

#pragma once


//=======
// Using
//=======

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")

#include "Graphics/Gdi/Icon.h"
#include "Menus/PopupMenu.h"


//======================
// Forward-Declarations
//======================

namespace Desktop
{
class Application;
class AppWindow;
}


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//===========
// Tray-Icon
//===========

class TrayIcon: public Object
{
private:
	// Using
	using PopupMenu=Menus::PopupMenu;

public:
	// Con-/Destructors
	TrayIcon(Handle<Application> Application, Handle<AppWindow> AppWindow, UINT Icon);
	~TrayIcon();

	// Common
	VOID Destroy();
	VariableHandle<TrayIcon, Graphics::Gdi::Icon> Icon;
	VariableHandle<TrayIcon, PopupMenu> Menu;
	VariableHandle<TrayIcon, String> Text;

private:
	// Common
	RECT GetRect();
	VOID OnApplicationUnhandledException(Handle<Application> Application, EXCEPTION_POINTERS* Info);
	VOID OnAppWindowMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Status);
	VOID OnIconChanged(Handle<Graphics::Gdi::Icon> Icon);
	VOID OnLButtonUp(Handle<TrayIcon> TrayIcon);
	VOID OnRButtonUp(Handle<TrayIcon> TrayIcon);
	VOID OnTextChanged(Handle<String> Text);
	Handle<Application> hApplication;
	Handle<AppWindow> hAppWindow;
	INT iShowCmd;
};

}}
