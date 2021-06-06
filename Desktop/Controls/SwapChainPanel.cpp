//====================
// SwapChainPanel.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/DirectX/ColorHelper.h"
#include "Container.h"
#include "SwapChainPanel.h"

using namespace Graphics::DirectX;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
	

//==================
// Con-/Destructors
//==================

SwapChainPanel::SwapChainPanel(Handle<Container> hparent, Handle<DirectXContext> hdc):
Interactive(hparent),
hDeviceContext(hdc)
{
Destroyed.Add(this, &SwapChainPanel::OnDestroyed);
MessageReceived.Add(this, &SwapChainPanel::OnMessageReceived);
Visible.Changed.Add(this, &SwapChainPanel::OnVisibleChanged);
WindowInfo info;
info.Class="SwapChainPanel";
info.ClassStyle=CS_OWNDC;
info.Style=WS_VISIBLE;
Create(info);
if(!hDeviceContext)
	{
	hDeviceContext=new DirectXContext();
	hDeviceContext->DeviceLost.Add(this, &SwapChainPanel::OnDeviceLost);
	hDeviceContext->DeviceRestored.Add(this, &SwapChainPanel::OnDeviceRestored);
	}
hSwapChain=new SwapChain(hDeviceContext, hWindow, GetDotsPerInch());
}


//================
// Device Private
//================

VOID SwapChainPanel::Destroy()
{
StopRendering();
if(hDeviceContext)
	{
	hDeviceContext->DeviceLost.Remove(this);
	hDeviceContext->DeviceRestored.Remove(this);
	hDeviceContext=nullptr;
	}
hSwapChain=nullptr;
}

VOID SwapChainPanel::DoRender()
{
while(!hRenderTask->Cancel)
	{
	auto cs=hDeviceContext->GetCriticalSection();
		{
		ScopedLock lock(cs);
		hDeviceContext->SetTarget(hSwapChain);
		FLOAT4 c=ColorToVector(BackgroundBrush->GetColor());
		hDeviceContext->Clear(c);
		Rendered(this);
		hSwapChain->Present();
		}
	hDeviceContext->WaitForVBlank();
	}
}

VOID SwapChainPanel::OnDeviceLost(Handle<DirectXContext> hdc)
{
StopRendering();
}

VOID SwapChainPanel::OnDeviceRestored(Handle<DirectXContext> hsender)
{
StartRendering();
}

VOID SwapChainPanel::StartRendering()
{
if(hRenderTask)
	return;
hRenderTask=CreateTask(this, &SwapChainPanel::DoRender);
}

VOID SwapChainPanel::StopRendering()
{
if(!hRenderTask)
	return;
hRenderTask->Cancel=true;
hRenderTask->Wait();
hRenderTask=nullptr;
}


//================
// Window Private
//================

VOID SwapChainPanel::OnDestroyed(Handle<Window> hwnd)
{
Destroy();
}

VOID SwapChainPanel::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
switch(umsg)
	{
	case WM_DPICHANGED:
		{
		UINT udpi=LOWORD(wparam);
		if(hSwapChain)
			hSwapChain->SetDotsPerInch(udpi);
		break;
		}
	case WM_SIZE:
		{
		if(!hSwapChain)
			break;
		::RECT rc;
		GetClientRect(hWindow, &rc);
		hSwapChain->SetSize(rc.right, rc.bottom);
		break;
		}
	}
}

VOID SwapChainPanel::OnVisibleChanged(BOOL bvisible)
{
BOOL b=Visible;
b? StartRendering(): StopRendering();
}

}}
