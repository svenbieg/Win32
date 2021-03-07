//===========
// EditBox.h
//===========

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


//==========
// Edit-Box
//==========

class EditBox: public Interactive
{
public:
	// Con-/Destructors
	EditBox(Handle<Container> Parent);
	~EditBox();

	// Common
	VOID Clear();
	Handle<String> GetText();
	Property<EditBox, BOOL> ReadOnly;
	Event<EditBox> TextChanged;
	VOID Write(Handle<String> Text);
	VOID WriteLine(Handle<String> Text);

private:
	// Common
	VOID OnParentCommand(Handle<Window> Sender, Handle<Control> Control, UINT Command, BOOL& Handled);
	VOID OnReadOnlyChanged(BOOL ReadOnly);
};

}}