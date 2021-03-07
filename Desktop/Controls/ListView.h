//============
// ListView.h
//============

#pragma once


//=======
// Using
//=======

#pragma comment(lib,"comctl32.lib")

#include <CommCtrl.h>
#include "Collections/Vector.h"
#include "Graphics/Gdi/ImageList.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//======
// Mode
//======

enum class ListViewMode: UINT
{
Details=LV_VIEW_DETAILS,
Icon=LV_VIEW_ICON,
List=LV_VIEW_LIST,
SmallIcon=LV_VIEW_SMALLICON,
Tile=LV_VIEW_TILE
};


//===========
// List-View
//===========

class ListView: public Control
{
private:
	// Using
	using ImageList=Graphics::Gdi::ImageList;
	using SelectionList=Collections::Vector<UINT>;

public:
	// Con-/Destructors
	ListView(Handle<Container> Parent);
	~ListView();

	// Common
	UINT AddColumn(Handle<String> Text, UINT Width);
	UINT AddIcon(UINT Normal, UINT State=0);
	Event<ListView, INT, Handle<String>> BeginLabelEdit;
	VOID Clear();
	VOID ClearIcons();
	VOID ClearItems();
	VOID ClearSelection();
	Handle<Bool> EditLabels;
	Event<ListView, INT, Handle<String>, Handle<String>> EndLabelEdit;
	Handle<Int32> FocusItem;
	UINT GetColumnCount()const { return uColumnCount; }
	UINT GetColumnWidth(UINT Id);
	UINT GetItemCount();
	Handle<String> GetItemText(UINT Item, UINT Column);
	ListViewMode GetMode();
	UINT GetSelectedCount();
	Handle<SelectionList> GetSelection();
	UINT InsertColumn(INT Position, LVCOLUMN const& Info);
	UINT InsertItem(INT Position, Handle<String> Text);
	UINT InsertItem(INT Position, Handle<String> Text, UINT Icon);
	Event<ListView, INT> ItemActivated;
	Event<ListView, INT> ItemDeleted;
	VOID RemoveItem(UINT Item);
	Event<ListView, INT> SelectionChanged;
	VOID SetItemIcon(UINT Item, UINT Icon);
	VOID SetItemText(UINT Item, UINT Column, Handle<String> Text);
	VOID SetMode(ListViewMode Mode);
	VOID SortItems(UINT Column);

private:
	// Common
	static INT CALLBACK CompareItems(LPARAM LParam1, LPARAM LParam2, LPARAM ListView);
	VOID OnEditLabelsChanged(Handle<Variable> Variable);
	VOID OnEditLabelsReading(Handle<Variable> Variable, BOOL& Value);
	VOID OnFocusItemChanged(Handle<Variable> Variable);
	VOID OnFocusItemReading(Handle<Variable> Variable, INT& Value);
	VOID OnParentNotified(Handle<Window> Sender, NMHDR const* Info, BOOL& Handled, LRESULT& Status);
	Handle<ImageList> hImgListNormal;
	Handle<ImageList> hImgListState;
	Handle<String> hOldLabel;
	UINT uColumnCount;
	UINT uSortColumn;
};

}}
