//================
// SimpleDialog.h
//================

#pragma once


//=======
// Using
//=======

#include "Desktop/Window.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//===============
// Simple Dialog
//===============

class SimpleDialog
{
public:
	// Common
	static INT Show(Handle<Window> Parent, Handle<String> Title, Handle<String> Message, UINT Buttons=MB_OK, HRESULT Status=S_OK);
};

}}
