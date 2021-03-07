//============
// CheckBox.h
//============

#pragma once


//=======
// Using
//=======

#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//===========
// Check-Box
//===========

class CheckBox: public Interactive
{
public:
	// Con-/Destructors
	CheckBox(Handle<Container> Parent, Handle<String> Text);
	~CheckBox();

	// Common
	Property<CheckBox, BOOL> Checked;
	VariableHandle<CheckBox, String> Text;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnCheckedChanged(BOOL Checked);
	VOID OnParentCommand(Handle<Window> Sender, Handle<Control> Control, UINT Command, BOOL& Handled);
	VOID OnTextChanged(Handle<String> Text);
	BOOL bChanging;
};

}}