//============
// Splitter.h
//============

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


//=============
// Orientation
//=============

enum class SplitterOrientation
{
Horizontal, Vertical
};


//==========
// Splitter
//==========

class Splitter: public Container
{
public:
	// Con-/Destructors
	Splitter(Handle<Container> Parent, SplitterOrientation Orientation=SplitterOrientation::Vertical);

	// Common
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;

private:
	// Common
	VOID OnMessageReceived(Handle<Window> Sender, UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled, LRESULT& Result);
	VOID OnLButtonDown(POINT const& Point);
	VOID OnLButtonUp(POINT const& Point);
	VOID OnPointerMoved(POINT const& Point);
	SplitterOrientation cOrientation;
	INT iCurrentSize;
	INT iMaxSize0;
	INT iMaxSize1;
	INT iMinSize0;
	INT iMinSize1;
	INT iSize;
	INT iStartSize;
	::POINT ptStart;
};

}}
