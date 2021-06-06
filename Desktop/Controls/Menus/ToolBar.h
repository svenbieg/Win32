//===========
// ToolBar.h
//===========

#pragma once


//=======
// Using
//=======

#pragma comment(lib, "comctl32.lib")

#include <CommCtrl.h>
#include "ToolBand.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==========
// Tool-Bar
//==========

class ToolBar: public Container
{
private:
	// Using
	using BandList=Collections::Cluster<Handle<ToolBand>>;

public:
	// Con-/Destructors
	ToolBar(Handle<Container> Parent);

	// Common
	Handle<ToolBand> AddBand(WORD Id) { return InsertBand(-1, Id); }
	VOID CheckButton(WORD Id, BOOL Checked);
	VOID EnableButton(WORD Id, BOOL Enabled);
	Handle<ToolBand> GetBand(WORD Id);
	UINT GetBandCount();
	BOOL GetBandInfo(WORD Id, REBARBANDINFO& Info);
	UINT GetBandPosition(WORD Id);
	Handle<ToolBand> InsertBand(INT Position, WORD Id);
	BOOL IsBandVisible(WORD Id);
	BOOL IsButtonChecked(WORD Id);
	BOOL IsButtonEnabled(WORD Id);
	VOID ShowBand(WORD Id, BOOL Visible);
	VOID ShowButton(WORD Id, BOOL Visible);

private:
	// Common
	//HWND GetBandHandle(UINT Position);
	//UINT GetBandWidth(HWND Window);
	VOID OnMoved(RECT const& Rect);
	VOID OnMoving(RECT& Rect);
	VOID RearrangeBands();
};

}}}
