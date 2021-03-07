//=================
// CommandLine.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include <shellapi.h>
#include "CommandLine.h"


//==================
// Con-/Destructors
//==================

CommandLine::CommandLine():
ppArgs(nullptr),
uCount(0)
{
LPWSTR pcmdline=GetCommandLineW();
INT icount=0;
ppArgs=CommandLineToArgvW(pcmdline, &icount);
uCount=icount;
}


//========
// Common
//========

Handle<CommandLine> CommandLine::Current=new CommandLine();

LPCWSTR CommandLine::GetArgument(UINT uid)const
{
if(uid>=uCount)
	return nullptr;
return ppArgs[uid];
}

BOOL CommandLine::HasArgument(Handle<String> harg)
{
if(!harg)
	return false;
for(UINT u=0; u<uCount; u++)
	{
	if(StringCompare(harg->Begin(), ppArgs[u], 0, false)==0)
		return true;
	}
return false;
}
