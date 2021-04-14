//==============
// SerialPort.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/RandomAccessStream.h"
#include "Devices/Serial/BaudRate.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//=============
// Serial-Port
//=============

class SerialPort: public ::Storage::Streams::RandomAccessStream
{
public:
	// Con-/Destructors
	SerialPort(UINT Id=0, BaudRate Baud=BaudRate::Baud115200);

	// Common
	Event<SerialPort> DataReceived;
	VOID Listen();

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Common
	VOID DoListen();
	CriticalSection cCriticalSection;
	HANDLE hInput;
	Handle<Task> hListenTask;
	HANDLE hOutput;
};

}}
