//==================
// OpenFileDialog.h
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
// Open File
//===========

class OpenFileDialog
{
public:
	// Common
	static Handle<String> Show(Handle<Window> Parent, Handle<String> Title, Handle<String> Path, Handle<String> Filter=nullptr, UINT Flags=0);
};

}}
