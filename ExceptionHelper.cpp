//=====================
// ExceptionHelper.cpp
//=====================

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "Dbghelp.lib")

#include <DbgHelp.h>
#include <TlHelp32.h>
#include "ExceptionHelper.h"


//==========
// Settings
//==========

const UINT uMaxSymNameLen=32;


//=============
// Environment
//=============

Handle<String> GetErrorContextString(CONTEXT* pc)
{
LoadSymbols();
HANDLE hproc=GetCurrentProcess();
HANDLE hthread=GetCurrentThread();
CONTEXT context;
CopyMemory(&context, pc, sizeof(CONTEXT));
STACKFRAME64 sf;
ZeroMemory(&sf, sizeof(STACKFRAME64));
#ifdef _ARM_
DWORD dwarch=IMAGE_FILE_MACHINE_ARM;
sf.AddrFrame.Offset=0;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=0;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Sp;
sf.AddrStack.Mode=AddrModeFlat;
DebugBreak();
#endif
#ifdef _X86_
DWORD dwarch=IMAGE_FILE_MACHINE_I386;
sf.AddrFrame.Offset=context.Ebp;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=context.Eip;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Esp;
sf.AddrStack.Mode=AddrModeFlat;
#endif
#ifdef _AMD64_
DWORD dwarch=IMAGE_FILE_MACHINE_AMD64;
sf.AddrFrame.Offset=context.Rsp;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=context.Rip;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Rsp;
sf.AddrStack.Mode=AddrModeFlat;
#endif
auto psym=(SYMBOL_INFO*)Alloc(sizeof(SYMBOL_INFO)+uMaxSymNameLen);
ZeroMemory(psym, sizeof(SYMBOL_INFO));
psym->SizeOfStruct=sizeof(SYMBOL_INFO);
psym->MaxNameLen=uMaxSymNameLen;
CHAR pcontext[1024];
pcontext[0]=0;
UINT ulen=0;
for(UINT uframe=0; ; uframe++)
	{
	if(!StackWalk64(dwarch, hproc, hthread, &sf, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0))
		break;
	if(sf.AddrPC.Offset==sf.AddrReturn.Offset)
		break;
	SetLastError(0);
	if(!SymFromAddr(hproc, sf.AddrPC.Offset, 0, psym))
		{
		DWORD dwerr=GetLastError();
		continue;
		}
	ulen+=StringCopy(&pcontext[ulen], 1022-ulen, psym->Name);
	pcontext[ulen++]='\n';
	pcontext[ulen]=0;
	if(ulen>=1022)
		break;
	}
Free(psym);
return pcontext;
}

VOID LoadSymbols()
{
HANDLE hproc=GetCurrentProcess();
if(!SymInitialize(hproc, 0, false))
	{
	DWORD dwerr=GetLastError();
	ExitProcess(dwerr);
	}
if(!SymSetOptions(SYMOPT_INCLUDE_32BIT_MODULES))
	{
	DWORD dwerr=GetLastError();
	ExitProcess(dwerr);
	}
SetLastError(0);
HANDLE hsnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
DWORD dwerr=GetLastError();
if(hsnapshot==INVALID_HANDLE_VALUE)
	hsnapshot=0;
if(!hsnapshot)
	{
	if(!dwerr)
		dwerr=ERROR_UNIDENTIFIED_ERROR;
	ExitProcess(dwerr);
	}
MODULEENTRY32 mod;
ZeroMemory(&mod, sizeof(MODULEENTRY32));
mod.dwSize=sizeof(MODULEENTRY32);
if(!Module32First(hsnapshot, &mod))
	ExitProcess(ERROR_UNIDENTIFIED_ERROR);
while(mod.hModule)
	{
	#ifdef _UNICODE
	CHAR pmod[MAX_PATH];
	StringCopy(pmod, MAX_PATH, mod.szModule);
	#else
	LPSTR pmod=mod.szModule;
	#endif
	SetLastError(0);
	SymLoadModule64(hproc, 0, (LPCSTR)pmod, 0, (UINT64)mod.modBaseAddr, mod.modBaseSize);
	if(!Module32Next(hsnapshot, &mod))
		break;
	}
CloseHandle(hsnapshot);
CloseHandle(hproc);
}
