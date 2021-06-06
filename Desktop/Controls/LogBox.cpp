//============
// LogBox.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Filesystem/FileHelper.h"
#include "Container.h"
#include "LogBox.h"

using namespace Storage;
using namespace Storage::Filesystem;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

LogBox::LogBox(Handle<Container> hparent, Handle<String> hpath):
Control(hparent)
{
WindowInfo info;
info.BackgroundBrush=new GdiBrush(GetSysColor(COLOR_WINDOW));
info.Class="EDIT";
info.Style=ES_MULTILINE|WS_VISIBLE|WS_VSCROLL;
Create(info);
SendMessage(hWindow, EM_SETREADONLY, true, 0);
if(hpath)
	{
	hFile=new TextFile(hpath);
	if(!hFile->Create(FileCreateMode::OpenAlways, FileAccessMode::ReadWrite))
		hFile=nullptr;
	if(hFile)
		{
		auto htext=hFile->Read();
		SendMessage(hWindow, WM_SETTEXT, 0, (LPARAM)htext->Begin());
		UINT ulen=GetWindowTextLength(hWindow);
		SendMessage(hWindow, EM_SETSEL, ulen, ulen);
		SendMessage(hWindow, EM_SCROLLCARET, 0, 0);
		}
	}
}


//========
// Common
//========

VOID LogBox::Clear()
{
SendMessage(hWindow, WM_SETTEXT, 0, 0);
if(hFile)
	hFile->Clear();
}

VOID LogBox::SetText(Handle<String> htext)
{
Clear();
if(!htext)
	return;
SendMessage(hWindow, WM_SETTEXT, 0, (LPARAM)htext->Begin());
UINT ulen=GetWindowTextLength(hWindow);
SendMessage(hWindow, EM_SETSEL, ulen, ulen);
SendMessage(hWindow, EM_SCROLLCARET, 0, 0);
if(hFile)
	hFile->Write(htext);
}

VOID LogBox::Write(Handle<String> htext)
{
if(!htext)
	return;
UINT ulen=GetWindowTextLength(hWindow);
SendMessage(hWindow, EM_SETSEL, ulen, ulen);
SYSTEMTIME time;
GetLocalTime(&time);
Handle<String> hmsg=new String("%i-%02i-%02i %02i:%02i - %s\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, htext->Begin());
SendMessage(hWindow, EM_REPLACESEL, 0, (LPARAM)hmsg->Begin());
ulen=GetWindowTextLength(hWindow);
SendMessage(hWindow, EM_SETSEL, ulen, ulen);
SendMessage(hWindow, EM_SCROLLCARET, 0, 0);
if(hFile)
	hFile->Append(hmsg);
}

}}
