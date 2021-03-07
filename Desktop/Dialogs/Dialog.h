//==========
// Dialog.h
//==========

#pragma once


//=======
// Using
//=======

#include "Desktop/Application.h"
#include "Desktop/Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Dialog
//========

class Dialog: public Overlapped
{
public:
	// Common
	VOID EndDialog(UINT Id) { ::EndDialog(hWindow, Id); }
	Handle<GdiIcon> GetIcon() { return hIcon; }
	HWND GetItem(UINT Id) { return GetDlgItem(hWindow, Id); }
	Event<Dialog> Initialized;
	INT_PTR Show();
	VOID ShowModeless();
	VOID SetIcon(Handle<GdiIcon> Icon);

protected:
	// Con-/Destructors
	Dialog(Handle<Container> Parent, UINT Style, Handle<String> Title, UINT Icon);

private:
	// Template
	typedef struct
		{
		DLGTEMPLATE dthHeader;
		WORD uMenu;
		WORD uClass;
		WCHAR pTitle[80];
		}TEMPLATE;

	// Common
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT Msg, WPARAM WParam, LPARAM LParam);
	VOID OnClosed(Handle<Window> Window);
	VOID OnDestroyed(Handle<Window> Window);
	VOID OnParentVisibleChanged(BOOL Visible);
	VOID Reposition();
	BOOL bModeless;
	TEMPLATE cTemplate;
	Handle<GdiIcon> hIcon;
};

}}
