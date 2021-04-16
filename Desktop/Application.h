//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Core/Application.h"
#include "AppWindow.h"
#include "CommandLine.h"


//===========
// Namespace
//===========

namespace Desktop {


//=============
// Application
//=============

class Application: public Core::Application
{
public:
	// Common
	Handle<AppWindow> AppWindow;
	Property<Application, BOOL> AutoStart;
	Handle<CommandLine> CommandLine;
	static Handle<Application> Current;
	Event<Application> Destroyed;
	Handle<String> GetPath();
	Event<Application, EXCEPTION_POINTERS*> UnhandledException;

protected:
	// Con-/Destructors
	Application(Handle<String> Name);

private:
	// Common
	VOID OnAutoStartChanged(BOOL AutoStart);
	VOID OnAutoStartReading(BOOL& AutoStart);
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* Pointers);
	LPTOP_LEVEL_EXCEPTION_FILTER pExceptionFilter;
};

}
