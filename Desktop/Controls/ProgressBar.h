//===============
// ProgressBar.h
//===============

#pragma once


//=======
// Using
//=======

#pragma comment(lib, "comctl32.lib")

#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==============
// Progress-Bar
//==============

class ProgressBar: public Control
{
public:
	// Con-/Destructors
	ProgressBar(Handle<Container> Parent);

	// Common
	Property<ProgressBar, FLOAT> Progress;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnMoved(Handle<CoreWindow> Sender, RECT const& Rect);
	VOID OnProgressChanged(FLOAT Progress);
	UINT uMax;
};

}}
