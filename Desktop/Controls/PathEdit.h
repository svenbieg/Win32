//============
// PathEdit.h
//============

#pragma once


//=======
// Using
//=======

#include "Button.h"
#include "Container.h"
#include "Edit.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=======
// Flags
//=======

enum class PathEditFlags: UINT
{
None=0,
IgnoreEdit=1,
IgnorePath=2
};


//===========
// Path-Edit
//===========

class PathEdit: public Container
{
public:
	// Con-/Destructors
	PathEdit(Handle<Container> Parent, Handle<String> Path=nullptr);

	// Common
	Property<PathEdit, BOOL> Button;
	BOOL Directory;
	Handle<String> GetPath();
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	VOID SetPath(Handle<String> Path);

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnButtonChanged(BOOL Button);
	VOID OnCommand(Handle<Window> Sender, Handle<Control> Control, UINT Command, BOOL& Handled);
	Handle<Controls::Button> hButton;
	Handle<Edit> hEdit;
	PathEditFlags uPathEditFlags;
};

}}
