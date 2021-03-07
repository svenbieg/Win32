//==================
// SwapChainPanel.h
//==================

#pragma once


//=======
// Using
//=======

#include "Graphics/DirectX/DeviceContext.h"
#include "Graphics/DirectX/SwapChain.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
	

//==================
// Swap-Chain-Panel
//==================

class SwapChainPanel: public Interactive
{
protected:
	// Using
	using DirectXContext=Graphics::DirectX::DeviceContext;
	using SwapChain=Graphics::DirectX::SwapChain;

public:
	// Con-/Destructors
	SwapChainPanel(Handle<Container> Parent, Handle<DirectXContext> DeviceContext=nullptr);

	// Common
	Handle<DirectXContext> GetDeviceContext() { return hDeviceContext; }
	Event<SwapChainPanel> Rendered;

protected:
	// Common
	Handle<DirectXContext> hDeviceContext;

private:
	// Device
	VOID Destroy();
	VOID DoRender();
	VOID OnDeviceLost(Handle<DirectXContext> DeviceContext);
	VOID OnDeviceRestored(Handle<DirectXContext> DeviceContext);
	VOID StartRendering();
	VOID StopRendering();
	Handle<Task> hRenderTask;
	Handle<SwapChain> hSwapChain;

	// Window
	VOID OnDestroyed(Handle<Window> Sender);
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID OnVisibleChanged(BOOL Visible);
};

}}
