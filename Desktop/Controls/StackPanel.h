//==============
// StackPanel.h
//==============

#pragma once


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=============
// Orientation
//=============

enum class StackOrientation
{
Horizontal, Vertical
};


//=============
// Stack-Panel
//=============

class StackPanel: public Container
{
public:
	// Con-/Destructors
	StackPanel(Handle<Container> Parent, StackOrientation Orientation=StackOrientation::Horizontal);

	// Common
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	StackOrientation uOrientation;
};

}}
