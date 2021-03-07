//===============
// CommandLine.h
//===============

#pragma once


//==============
// Command-Line
//==============

class CommandLine: public Object
{
public:
	// Con-/Destructors
	CommandLine();

	// Common
	static Handle<CommandLine> Current;
	LPCWSTR GetArgument(UINT Id)const;
	UINT GetArgumentCount()const { return uCount; }
	BOOL HasArgument(Handle<String> Argument);

private:
	// Common
	LPWSTR* ppArgs;
	UINT uCount;
};
