//===========
// Frame.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Frame.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Frame::Frame(Handle<Container> hparent, Handle<CoreControl> hcontent, Handle<DirectXContext> hdc):
SwapChainPanel(hparent, hdc),
Content(hcontent)
{
KeyDown.Add(this, &Frame::OnKeyDown);
KeyUp.Add(this, &Frame::OnKeyUp);
PointerDown.Add(this, &Frame::OnPointerDown);
PointerMoved.Add(this, &Frame::OnPointerMoved);
PointerUp.Add(this, &Frame::OnPointerUp);
PointerWheel.Add(this, &Frame::OnPointerWheel);
Rendered.Add(this, &Frame::OnRendered);
WindowInfo info;
info.Class="Frame";
info.ClassStyle=CS_OWNDC;
info.Style=WS_VISIBLE;
Create(info);
}


//==================
// Common Protected
//==================

Graphics::SIZE Frame::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
if(Content)
	{
	SIZE ctrlsize=Content->GetMinSize(hdc);
	minsize.Width+=ctrlsize.Width;
	minsize.Height+=ctrlsize.Height;
	}
return minsize;
}


//================
// Common Private
//================

VOID Frame::OnKeyDown(VirtualKey key)
{
if(Content)
	Content->KeyDown(Content, key);
}

VOID Frame::OnKeyUp(VirtualKey key)
{
if(Content)
	Content->KeyUp(Content, key);
}

VOID Frame::OnPointerDown(POINT pt, UINT id)
{
if(Content)
	Content->PointerDown(Content, pt, id);
}

VOID Frame::OnPointerMoved(POINT pt, UINT id)
{
if(Content)
	Content->PointerMoved(Content, pt, id);
}

VOID Frame::OnPointerUp(POINT pt, UINT id)
{
if(Content)
	Content->PointerUp(Content, pt, id);
}

VOID Frame::OnPointerWheel(POINT pt, INT idelta)
{
if(Content)
	Content->PointerWheel(Content, pt, idelta);
}

VOID Frame::OnRendered(Handle<SwapChainPanel> hpanel)
{
if(Content)
	{
	RECT rc(0, 0, rcRect.Right-rcRect.Left, rcRect.Bottom-rcRect.Top);
	Content->Move(rc);
	//Content->Render(hDeviceContext);
	}
}

}}