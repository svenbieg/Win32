//=========
// Group.h
//=========

#pragma once


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=======
// Group
//=======

class Group: public Container
{
public:
	// Con-/Destructors
	Group(Handle<Container> Parent, Handle<String> Title);

	// Common
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	VariableHandle<Group, String> Title;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnPainted(Handle<Window> Sender, Handle<GdiContext> DeviceContext);
	VOID OnTitleChanged(Handle<String> Title);
};

}}
