//========================
// SelectFolderDialog.cpp
//========================

#include "pch.h"


//=======
// Using
//=======

#include <ShlObj.h>
#include "SelectFolderDialog.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

Handle<String> SelectFolderDialog::Show(Handle<Window> howner, Handle<String> hpath)
{
ASSERT(howner&&hpath);
TCHAR ppath[MAX_PATH];
StringCopy(ppath, MAX_PATH, hpath->Begin());
BROWSEINFO cinfo;
ZeroMemory(&cinfo, sizeof(BROWSEINFO));
cinfo.hwndOwner=howner->GetHandle();
cinfo.lpfn=DialogProc;
cinfo.lParam=(LPARAM)ppath;
cinfo.ulFlags=BIF_USENEWUI;
auto pids=SHBrowseForFolder(&cinfo);
if(!pids)
	return nullptr;
IShellFolder* pfolder=nullptr;
const ITEMIDLIST* pidchild=nullptr;
HRESULT hr=SHBindToParent(pids, IID_IShellFolder, (VOID**)&pfolder, &pidchild);
if(FAILED(hr))
	{
	CoTaskMemFree(pids);
	return nullptr;
	}
IShellLink* plink=nullptr;
hr=pfolder->GetUIObjectOf(NULL, 1, &pidchild, IID_IShellLink, nullptr, (VOID**)&plink);
pfolder->Release();
if(SUCCEEDED(hr))
	{
	CoTaskMemFree(pids);
	plink->GetIDList(&pids);
	}
if(SHGetPathFromIDList(pids, ppath))
	{
	UINT ulen=StringLength(ppath);
	if(ulen&&ppath[ulen-1]!='\\')
		{
		ppath[ulen++]='\\';
		ppath[ulen]=0;
		}
	}
else
	ppath[0]=0;
CoTaskMemFree(pids);
return new String(ppath);
}


//================
// Common Private
//================

INT SelectFolderDialog::DialogProc(HWND hwnd, UINT umsg, LPARAM lparam, LPARAM ldata)
{
switch(umsg)
	{
	case BFFM_INITIALIZED:
		{
		SendMessage(hwnd, BFFM_SETSELECTION, true, ldata);
		return 0;
		}
	}
return 0;
}

}}
