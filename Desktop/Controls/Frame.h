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
private:
	// Using
	using CoreControl=Core::Controls::Control;

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
	VOID OnKeyDown(VirtualKey Key);
	VOID OnKeyUp(VirtualKey Key);
	VOID OnPointerDown(POINT Point, UINT Pointer);
	VOID OnPointerMoved(POINT Point, UINT Pointer);
	VOID OnPointerUp(POINT Point, UINT Pointer);
	VOID OnPointerWheel(POINT Point, INT Delta);
	VOID OnRendered(Handle<SwapChainPanel> Panel);
};

}}
