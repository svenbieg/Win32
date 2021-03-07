//=============
// Clipboard.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"


//===========
// Namespace
//===========

namespace Storage {


//===========
// Clipboard
//===========

class Clipboard: public Object
{
public:
	// Con-/Destructors
	Clipboard();
	~Clipboard();

	// Common
	Event<Clipboard> Changed;
	Handle<Buffer> GetData(UINT Format);
	Handle<Buffer> GetData(LPCWSTR Format);
	UINT GetFormat(LPCWSTR Format);
	Handle<String> GetText();
	BOOL IsFormatAvailable(UINT Format);
	BOOL IsFormatAvailable(LPCWSTR Format);
	VOID SetData(UINT Format, VOID const* Buffer, UINT Size);
	VOID SetData(LPCWSTR Format, VOID const* Buffer, UINT Size);
	VOID SetText(LPCWSTR Text);
	UINT RegisterFormat(LPCWSTR Format);

private:
	// Common
	static LRESULT CALLBACK WndProc(HWND Wnd, UINT Msg, WPARAM WParam, LPARAM LParam);
	ATOM aClass;
	HWND hWindow;
};

}
