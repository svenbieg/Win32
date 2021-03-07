//==================
// SaveFileDialog.h
//==================

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


//===========
// Save File
//===========

class SaveFileDialog
{
public:
	// Common
	static Handle<String> Show(Handle<Window> Parent, Handle<String> Title, Handle<String> Path, Handle<String> Filter=nullptr, UINT Flags=0);
};

}}
