//======================
// SelectFolderDialog.h
//======================

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
// Select Folder
//===============

class SelectFolderDialog
{
public:
	// Common
	static Handle<String> Show(Handle<Window> Owner, Handle<String> Path);

private:
	// Common
	static INT CALLBACK DialogProc(HWND Window, UINT Message, LPARAM Param, LPARAM Data);
};

}}
