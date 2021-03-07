//============
// ComboBox.h
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
// Combo-Box
//===========

class ComboBox: public Interactive
{
public:
	// Con-/Destructors
	ComboBox(Handle<Container> Parent, BOOL Static=true, UINT Lines=10);
	~ComboBox();

	// Common
	INT Add(Handle<String> Line);
	VOID Clear();
	Event<ComboBox> ClosedUp;
	UINT GetCount();
	Event<ComboBox> DroppedDown;
	Event<ComboBox> EditChanged;
	Handle<String> GetLine(UINT Line);
	UINT GetLines()const { return uLines; }
	INT GetSelection();
	Event<ComboBox> SelectionChanged;
	VOID Set(Handle<String> Lines);
	VOID SetLines(UINT Lines);
	VOID SetSelection(INT Selection);

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnMoving(Handle<CoreWindow> Sender, RECT& Rect);
	VOID OnParentCommand(Handle<Window> Sender, Handle<Control> Control, UINT Command, BOOL& Handled);
	UINT uLines;
};

}}