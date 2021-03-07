//===============
// Interactive.h
//===============

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Interactive.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=============
// Interactive
//=============

class Interactive: public Control, public virtual Core::Controls::Interactive
{
public:
	// Con-/Destructors
	Interactive(Handle<Container> Parent);

	// Common
	Property<Interactive, BOOL> TabStop;

private:
	// Common
	VOID OnCreated(Handle<DesktopWindow> Sender);
	VOID OnFocused(Handle<CoreInteractive> Sender);
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID OnTabStopChanged(BOOL TabStop);
};

}}