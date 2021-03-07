//============
// Keyboard.h
//============

#pragma once


//===========
// Namespace
//===========

namespace Devices {
	namespace Input {


//==========
// Keyboard
//==========

class Keyboard: public Object
{
public:
	// Con-/Destructors
	Keyboard();
	~Keyboard();

	// Common
	BOOL IsKeyDown(BYTE Key);
	Event<Keyboard> KeyDown;
	Event<Keyboard> KeyUp;

private:
	static LRESULT CALLBACK KeyboardProc(INT Code, WPARAM WParam, LPARAM LParam);
	HHOOK hHook;
};

}}
