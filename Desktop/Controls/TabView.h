//===========
// TabView.h
//===========

#pragma once


//=======
// Using
//=======

#pragma comment(lib, "comctl32.lib")

#include "Graphics/Gdi/ImageList.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==========
// Tab-View
//==========

class TabView: public Container
{
private:
	// Using
	using ImageList=Graphics::Gdi::ImageList;

public:
	// Con-/Destructors
	TabView(Handle<Container> Parent);
	~TabView();

	// Common
	UINT AddTab(Handle<Control> Control, Handle<String> Text, UINT Icon=0);
	VOID Clear();
	Handle<Control> GetCurrent() { return hCurrent; }
	UINT GetCurrentId();
	UINT GetTabCount();
	Handle<String> GetTabText(UINT Id);
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	VOID RemoveTab(UINT Id);
	VOID SetCurrent(UINT Id);
	Event<TabView> SelectionChanged;

private:
	// Common
	VOID OnParentItemDrawed(Handle<Window> Sender, DRAWITEMSTRUCT const* Info);
	VOID OnParentNotified(Handle<Window> Sender, NMHDR const* Info, BOOL& Handled, LRESULT& Status);
	Handle<Control> hCurrent;
	Handle<ImageList> hImgList;
};

}}