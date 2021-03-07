//=========
// Frame.h
//=========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Control.h"
#include "SwapChainPanel.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=======
// Frame
//=======

class Frame: public SwapChainPanel
{
public:
	// Con-/Destructors
	Frame(Handle<Container> Parent, Handle<CoreControl> Content=nullptr, Handle<DirectXContext> DeviceContext=nullptr);

	// Common
	Handle<CoreControl> Content;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnClicked(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnContextMenu(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnDoubleClicked(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnKeyDown(Handle<CoreInteractive> Sender, VirtualKey Key);
	VOID OnKeyUp(Handle<CoreInteractive> Sender, VirtualKey Key);
	VOID OnLButtonDown(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnLButtonUp(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnPointerMoved(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnPointerWheel(Handle<CoreInteractive> Sender, POINT const& Point, INT Delta);
	VOID OnRendered(Handle<SwapChainPanel> Panel);
	VOID OnSizeChanged(Handle<CoreInteractive> Sender, RECT& Rect);
};

}}
