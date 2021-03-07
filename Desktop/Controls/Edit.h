//========
// Edit.h
//========

#pragma once


//=======
// Using
//=======

#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==============
// Edit-Control
//==============

class Edit: public Interactive
{
public:
	// Con-/Destructors
	Edit(Handle<Container> Parent, Handle<String> Text=nullptr);
	~Edit();

	// Common
	UINT GetLimit();
	Handle<String> GetText();
	Property<Edit, BOOL> ReadOnly;
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	VOID SetLimit(UINT Length);
	VOID SetText(Handle<String> Text);
	Event<Edit> TextChanged;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	static LRESULT CALLBACK EditWndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	VOID OnEditFocusLost(Handle<Interactive> Sender) { FocusLost(this); }
	VOID OnEditFocusSet(Handle<Interactive> Sender) { Focused(this); }
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID OnReadOnlyChanged(BOOL ReadOnly);
	VOID OnTabStopChanged(BOOL TabStop);
	HWND hEdit;
	WNDPROC pOldWndProc;
};

}}