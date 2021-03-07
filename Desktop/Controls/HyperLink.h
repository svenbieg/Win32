//=============
// HyperLink.h
//=============

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//============
// Hyper-Link
//============

class HyperLink: public Interactive
{
public:
	// Using
	using COLOR=Graphics::COLOR;

	// Con-/Destructors
	HyperLink(Handle<Container> Parent, Handle<String> Text, Handle<String> Link);

	// Common
	Property<HyperLink, COLOR> Color;
	static Handle<GdiCursor> DefaultCursor;
	static Handle<GdiFont> DefaultFont;
	VariableHandle<HyperLink, String> Link;
	VariableHandle<HyperLink, String> Text;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnClicked(Handle<CoreInteractive> Sender, POINT const& Point);
	VOID OnPainted(Handle<Window> Window, Handle<GdiContext> DeviceContext);
	VOID OnTextChanged(Handle<String> Text);
};

}}