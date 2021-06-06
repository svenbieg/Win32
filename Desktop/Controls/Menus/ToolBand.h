//============
// ToolBand.h
//============

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Desktop/Controls/Container.h"
#include "Graphics/Gdi/ImageList.h"
#include "ToolBandItem.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class ToolBar;


//===========
// Tool-Band
//===========

class ToolBand: public Container
{
private:
	// Using
	using ImageList=Graphics::Gdi::ImageList;
	using ItemList=Collections::Cluster<Handle<ToolBandItem>>;

public:
	// Friends
	friend ToolBandItem;

	// Con-/Destructors
	ToolBand(Handle<ToolBar> Parent, INT Position, WORD Id);

	// Common
	Handle<ToolBandItem> AddButton(WORD Id, WORD Icon, WORD IconDisabled=0, Handle<String> Text=nullptr) { return InsertButton(-1, Id, Icon, IconDisabled, Text); }
	BOOL CheckButton(WORD Id, BOOL Checked);
	BOOL EnableButton(WORD Id, BOOL Enabled);
	WORD GetId()const { return uId; }
	Handle<ToolBandItem> GetItem(WORD Id);
	UINT GetItemCount();
	BOOL GetItemInfo(WORD Id, TBBUTTONINFO& Info);
	UINT GetItemPosition(WORD Id);
	UINT GetPosition();
	Handle<ToolBandItem> InsertButton(INT Position, WORD Id, WORD Icon, WORD IconDisabled=0, Handle<String> Text=nullptr);
	BOOL IsButtonChecked(WORD Id);
	BOOL IsButtonEnabled(WORD Id);
	Handle<ItemList> Items;
	Handle<ToolBar> Parent;
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	BOOL ShowButton(WORD Id, BOOL Visible);
	
private:
	// Common
	INT AddIcon(WORD Normal, WORD Disabled=0);
	VOID OnVisibleChanged(BOOL Visible);
	VOID OnVisibleReading(BOOL& Visible);
	Handle<ImageList> hImgList;
	Handle<ImageList> hImgListDisabled;
	ToolBar* pToolBar;
	WORD uId;
};

}}}
