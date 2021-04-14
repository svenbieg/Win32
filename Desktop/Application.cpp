//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/RegistryKey.h"
#include "Application.h"
#include "ExceptionHelper.h"
#include "PathHelper.h"

using namespace Storage;

extern VOID Initialize();


//=============
// Entry-Point
//=============

#ifdef _DESKTOP

INT WinMain(HINSTANCE hinst, HINSTANCE hprevinst, LPSTR pcmdline, INT icmdshow)
{
Initialize();
auto happ=Desktop::Application::Current;
happ_>Initialized(happ);
if(happ->AppWindow)
	happ->AppWindow->Show(icmdshow);
MSG msg;
BOOL bmsg=true;
while(bmsg)
	{
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
		if(msg.message==WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	happ->Loop();
	}
happ->Destroyed();
return 0;
}

#endif


//===========
// Namespace
//===========

namespace Desktop {


//===========
// Autostart
//===========

LPCSTR pAutoRunPath="Software\\Microsoft\\Windows\\CurrentVersion\\Run";


//========
// Common
//========

Handle<Application> Application::Current;

Handle<String> Application::GetPath()
{
TCHAR ppath[MAX_PATH];
GetModuleFileName(NULL, ppath, MAX_PATH);
return ppath;
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(Handle<String> hname):
Core::Application(hname),
AutoStart(this, false)
{
AutoStart.Changed.Add(this, &Application::OnAutoStartChanged);
AutoStart.Reading.Add(this, &Application::OnAutoStartReading);
CommandLine=new ::CommandLine();
Current=this;
pExceptionFilter=SetUnhandledExceptionFilter(UnhandledExceptionHandler);
auto hpath=GetPath();
UINT udir=PathGetDirectoryLength(hpath->Begin());
Handle<String> hdir=new String(udir, hpath->Begin());
SetCurrentDirectory(hdir->Begin());
}


//================
// Common Private
//================

VOID Application::OnAutoStartChanged(BOOL bautostart)
{
auto hautorun=new RegistryKey(HKEY_CURRENT_USER, pAutoRunPath);
if(bautostart)
	{
	Handle<String> hcmd=GetPath()+" /background";
	hautorun->SetString(Name, hcmd);
	}
else
	{
	hautorun->DeleteValue(Name);
	}
}

VOID Application::OnAutoStartReading(BOOL& bautostart)
{
auto hautorun=new RegistryKey(HKEY_CURRENT_USER, pAutoRunPath, false);
bautostart=hautorun->HasString(Name);
}

LONG WINAPI Application::UnhandledExceptionHandler(EXCEPTION_POINTERS* pep)
{
HRESULT hr=pep->ExceptionRecord->ExceptionCode;
CHAR psys[1024];
FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, 0, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), psys, 1024, 0);
auto hcontext=GetErrorContextString(pep->ContextRecord);
LPCSTR pmsg=psys;
Handle<String> hmsg;
if(hcontext)
	{
	hmsg=new String("%s\n\n%s", pmsg, hcontext->Begin());
	pmsg=hmsg->Begin();
	}
MessageBox(NULL, pmsg, Application::Current->Name->Begin(), MB_ICONERROR);
auto happ=Application::Current;
happ->UnhandledException(happ, pep);
ExitProcess(pep->ExceptionRecord->ExceptionCode);
return EXCEPTION_CONTINUE_SEARCH;
}

}
