//================
// ToolBandItem.h
//================

#pragma once


//=======
// Using
//=======

#include "Graphics/Rect.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class ToolBand;
class ToolBar;


//================
// Tool-Band-Item
//================

class ToolBandItem: public Object
{
private:
	// Using
	using RECT=Graphics::RECT;

public:
	// Con-/Destructors
	ToolBandItem(Handle<ToolBand> Parent, INT Position, WORD Id, WORD Icon, WORD IconDisabled=0, Handle<String> Text=nullptr);

	// Common
	UINT GetHeight();
	WORD GetId()const { return uId; }
	UINT GetPosition();
	RECT GetRect();
	UINT GetWidth();
	Handle<ToolBand> Parent;
	VariableHandle<ToolBandItem, String> Text;
	Property<ToolBandItem, BOOL> Visible;

private:
	// Common
	VOID OnTextChanged(Handle<String> Text);
	VOID OnVisibleChanged(BOOL Visible);
	VOID OnVisibleReading(BOOL& Visible);
	WORD uId;
};

}}}
