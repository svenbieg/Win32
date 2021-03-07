//============
// TreeView.h
//============

#pragma once


//=======
// Using
//=======

#pragma comment(lib,"comctl32.lib")

#include <CommCtrl.h>
#include "Collections/Cluster.h"
#include "Graphics/Gdi/ImageList.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//===========
// Tree-View
//===========

class TreeView: public Control
{
private:
	// Using
	using ImageList=Graphics::Gdi::ImageList;
	using ItemList=Collections::Cluster<HTREEITEM>;

public:
	// Con-/Destructors
	TreeView(Handle<Container> Parent, BOOL Border=true);

	// Common
	UINT AddIcon(UINT Normal, UINT State=0);
	HTREEITEM AddItem(HTREEITEM Parent, Handle<String> Text, UINT Icon);
	VOID Clear() { ClearItems(); ClearIcons(); }
	VOID ClearIcons() { hImgListNormal->Clear(); hImgListState->Clear(); }
	VOID ClearItems() { SendMessage(hWindow, TVM_DELETEITEM, 0, 0); }
	BOOL GetButtons() { return GetStyle(TVS_HASBUTTONS); }
	UINT GetChildCount(HTREEITEM Item);
	Handle<ItemList> GetChildItems(HTREEITEM Item);
	BOOL GetEditLabels() { return GetStyle(TVS_EDITLABELS); }
	BOOL GetItem(TVITEMEX& Item) { return (BOOL)SendMessage(hWindow, TVM_GETITEM, 0, (LPARAM)&Item); }
	UINT GetItemCount() { return (UINT)SendMessage(hWindow, TVM_GETCOUNT, 0, 0); }
	HTREEITEM GetItemHandle(UINT Id) { return (HTREEITEM)SendMessage(hWindow, TVM_MAPACCIDTOHTREEITEM, Id, 0); }
	UINT GetItemId(HTREEITEM Item) { return (INT)SendMessage(hWindow, TVM_MAPHTREEITEMTOACCID, (WPARAM)Item, 0); }
	Handle<String> GetItemText(HTREEITEM Item);
	UINT GetLevel(HTREEITEM Item);
	BOOL GetLinesAtRoot() { return GetStyle(TVS_LINESATROOT); }
	HTREEITEM GetNextItem(HTREEITEM Item, UINT Type) { return (HTREEITEM)SendMessage(hWindow, TVM_GETNEXTITEM, Type, (LPARAM)Item); }
	HTREEITEM GetParentItem(HTREEITEM Item, UINT Level=0);
	UINT GetSelCount() { return (UINT)SendMessage(hWindow, TVM_GETSELECTEDCOUNT, 0, 0); }
	HTREEITEM GetSelectedItem() { GetNextItem(0, TVGN_CARET); }
	Handle<ItemList> GetSelectedItems();
	BOOL GetShowSelectionAlways() { return GetStyle(TVS_SHOWSELALWAYS); }
	HTREEITEM InsertItem(TVINSERTSTRUCT const& Info);
	VOID RemoveItem(HTREEITEM Item) { SendMessage(hWindow, TVM_DELETEITEM, 0, (LPARAM)Item); }
	VOID SelectItem(HTREEITEM Item) { SendMessage(hWindow, TVM_SELECTITEM, TVGN_CARET, (LPARAM)Item); }
	VOID SetButtons(BOOL Buttons) { SetStyle(TVS_HASBUTTONS, Buttons); }
	VOID SetEditLabels(BOOL EditLabels) { SetStyle(TVS_EDITLABELS, EditLabels); }
	VOID SetItem(TVITEMEX const& Item) { SendMessage(hWindow, TVM_SETITEM, 0, (LPARAM)&Item); }
	VOID SetItemText(HTREEITEM Item, Handle<String> Text);
	VOID SetLinesAtRoot(BOOL LinesAtRoot) { SetStyle(TVS_LINESATROOT, LinesAtRoot); }
	VOID SetShowSelectionAlways(BOOL ShowSelectionAlways) { SetStyle(TVS_SHOWSELALWAYS, ShowSelectionAlways); }

private:
	// Common
	Handle<ImageList> hImgListNormal;
	Handle<ImageList> hImgListState;
};

}}
