//==================
// ProgressDialog.h
//==================

#pragma once


//=======
// Using
//=======

#include "Desktop/Controls/Button.h"
#include "Desktop/Controls/Grid.h"
#include "Desktop/Controls/Group.h"
#include "Desktop/Controls/ProgressBar.h"
#include "Desktop/Controls/TextBlock.h"
#include "Dialog.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//=================
// Progress-Dialog
//=================

class ProgressDialog: public Dialog
{
public:
	// Con-/Destructors
	ProgressDialog(Handle<Container> Parent, Handle<String> Title, UINT Icon);

	// Common
	VariableHandle<ProgressDialog, String> Cancel;
	VariableHandle<ProgressDialog, String> Element;
	VariableHandle<ProgressDialog, String> ElementFooter;
	VariableHandle<ProgressDialog, String> ElementHeader;
	Property<ProgressDialog, FLOAT> ElementProgress;
	VariableHandle<ProgressDialog, String> Total;
	VariableHandle<ProgressDialog, String> TotalFooter;
	VariableHandle<ProgressDialog, String> TotalHeader;
	Property<ProgressDialog, FLOAT> TotalProgress;

private:
	// Common
	VOID OnCancelChanged(Handle<String> Cancel);
	VOID OnElementChanged(Handle<String> Element);
	VOID OnElementFooterChanged(Handle<String> ElementFooter);
	VOID OnElementHeaderChanged(Handle<String> ElementHeader);
	VOID OnElementProgressChanged(FLOAT Progress);
	VOID OnInitialized(Handle<Dialog> Dialog);
	VOID OnTotalChanged(Handle<String> Total);
	VOID OnTotalFooterChanged(Handle<String> TotalFooter);
	VOID OnTotalHeaderChanged(Handle<String> TotalHeader);
	VOID OnTotalProgressChanged(FLOAT Progress);
	Handle<Controls::Button> hCancelButton;
	Handle<Controls::Grid> hCancelGrid;
	Handle<Controls::ProgressBar> hElementBar;
	Handle<Controls::TextBlock> hElementFooter;
	Handle<Controls::Grid> hElementGrid;
	Handle<Controls::Group> hElementGroup;
	Handle<Controls::TextBlock> hElementHeader;
	Handle<Controls::Grid> hGrid;
	Handle<Controls::ProgressBar> hTotalBar;
	Handle<Controls::TextBlock> hTotalFooter;
	Handle<Controls::Grid> hTotalGrid;
	Handle<Controls::Group> hTotalGroup;
	Handle<Controls::TextBlock> hTotalHeader;
};

}}
