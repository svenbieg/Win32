//==========
// Window.h
//==========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Control.h"
#include "Graphics/Gdi/Brush.h"
#include "Graphics/Gdi/Cursor.h"
#include "Graphics/Gdi/DeviceContext.h"
#include "Graphics/Gdi/Font.h"
#include "Graphics/Gdi/Icon.h"


//======================
// Forward-Declarations
//======================

namespace Desktop
{
namespace Controls
	{
	class Container;
	}
}


//===========
// Namespace
//===========

namespace Desktop {


//=============
// Window-Info
//=============

typedef struct WindowInfo
{
// Con-/Destructors
WindowInfo():
	Class(nullptr),
	ClassStyle(0),
	Height(1),
	Icon(0),
	IconSm(0),
	Id(0),
	Left(0),
	Style(0),
	StyleEx(0),
	Text(nullptr),
	Top(0),
	Width(1)
	{}

// Common
Handle<Graphics::Gdi::Brush> BackgroundBrush;
LPCTSTR Class;
UINT ClassStyle;
Handle<Graphics::Gdi::Cursor> Cursor;
Handle<Graphics::Gdi::Font> Font;
UINT Height;
WORD Icon;
WORD IconSm;
UINT Id;
UINT Left;
UINT Style;
UINT StyleEx;
Handle<String> Text;
UINT Top;
UINT Width;
}WindowInfo;


//========
// Window
//========

class Window: public virtual Core::Controls::Control
{
public:
	// Using
	using Container=Desktop::Controls::Container;
	using GdiBrush=Graphics::Gdi::Brush;
	using GdiContext=Graphics::Gdi::DeviceContext;
	using GdiCursor=Graphics::Gdi::Cursor;
	using GdiFont=Graphics::Gdi::Font;
	using GdiIcon=Graphics::Gdi::Icon;

public:
	// Common
	VariableHandle<Window, GdiBrush> BackgroundBrush;
	Event<Window> Created;
	Handle<GdiCursor> Cursor;
	VOID Destroy();
	Event<Window> Destroyed;
	WORD GetDotsPerInch()const { return uDotsPerInch; }
	HWND GetHandle()const { return hWindow; }
	Handle<Container> GetParent()const;
	VOID* GetProperty(Handle<String> Name);
	UINT GetStyle();
	BOOL GetStyle(UINT Flag);
	UINT GetStyleEx();
	VOID Invalidate();
	Event<Window, UINT, WPARAM, LPARAM, BOOL&, LRESULT&> MessageReceived;
	Event<Window, Handle<GdiContext>> Painted;
	virtual VOID Repaint();
	VOID SetProperty(Handle<String> Name, VOID* Value);
	VOID SetStyle(UINT Style);
	VOID SetStyle(UINT Flag, BOOL Set);
	VOID SetStyleEx(UINT Style);
	WNDPROC SetWindowProc(WNDPROC Proc);
	Event<Window, WORD> TimerTriggered;

protected:
	// Con-/Destructors
	Window(Handle<Container> Parent);

	// Common
	VOID Create(WindowInfo const& Info);
	HWND hWindow;
	Container* pParent;

private:
	// Common
	static LRESULT CALLBACK ClassWndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	VOID OnBackgroundBrushChanged(Handle<GdiBrush> BackgroundBrush);
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID OnMoved(RECT const& Rect);
	VOID OnVisibleChanged(BOOL Visible);
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	Handle<Window> hThis;
	WNDPROC pOldWndProc;
	WORD uDotsPerInch;
};

}