//=========
// Mouse.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Devices {
	namespace Input {


//=======
// Mouse
//=======

class Mouse: public Object
{
public:
	// Con-/Destructors
	Mouse();
	~Mouse();

	// Common
	BOOL IsLButtonDown() { return GetFlag(uFlags, MouseFlags::LButton); }
	BOOL IsMButtonDown() { return GetFlag(uFlags, MouseFlags::MButton); }
	BOOL IsRButtonDown() { return GetFlag(uFlags, MouseFlags::RButton); }
	Event<Mouse> LButtonDown;
	Event<Mouse> LButtonUp;
	Event<Mouse> MButtonDown;
	Event<Mouse> MButtonUp;
	Event<Mouse> RButtonDown;
	Event<Mouse> RButtonUp;
	Event<Mouse> WheelChanged;

private:
	// Flags
	enum class MouseFlags: UINT
		{
		None=0,
		LButton=1,
		MButton=2,
		RButton=4
		};

	// Common
	static LRESULT CALLBACK MouseProc(INT Code, WPARAM WParam, LPARAM LParam);
	HHOOK hHook;
	MouseFlags uFlags;
};

}}
