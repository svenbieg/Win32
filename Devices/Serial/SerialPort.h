//==============
// SerialPort.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/OutputStream.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//=============
// Serial-Port
//=============

class SerialPort: public ::Storage::Streams::OutputStream
{
public:
	// Con-/Destructors
	SerialPort();

	// Common
	static Handle<SerialPort> Current;
	VOID Flush()override {}
	VOID Print(LPCSTR Value);
	VOID Print(LPCWSTR Value);
	VOID Print(Handle<String> Value);
	VOID Print(UINT Length, LPCSTR Text);
	VOID Print(UINT Length, LPCWSTR Text);
	template <class... _args_t> VOID Print(LPCSTR Format, _args_t... Arguments) { Print(new String(Format, Arguments...)); }
	template <class... _args_t> VOID Print(LPCWSTR Format, _args_t... Arguments) { Print(new String(Format, Arguments...)); }
	Handle<String> Read();
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Common
	VOID DoPrint(UINT Length, LPCSTR Value);
	VOID DoPrint(UINT Length, LPCWSTR Value);
	SIZE_T DoWrite(BYTE const* Buffer, SIZE_T Size);
	CriticalSection cCriticalSection;
	HANDLE hInput;
	HANDLE hOutput;
};

}}
