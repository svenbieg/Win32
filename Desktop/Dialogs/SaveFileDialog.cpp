//====================
// SaveFileDialog.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include <commdlg.h>
#include "SaveFileDialog.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

Handle<String> SaveFileDialog::Show(Handle<Window> hparent, Handle<String> htitle, Handle<String> hpath, Handle<String> hfilter, UINT uflags)
{
ASSERT(hparent&&htitle&&hpath);
HWND hwnd=hparent->GetHandle();
Handle<String> hfilter1;
LPTSTR pfilter=nullptr;
if(hfilter)
	{
	hfilter1=new String(hfilter->Begin());
	pfilter=(LPTSTR)hfilter1->Begin();
	UINT ulen=StringLength(pfilter);
	for(UINT u=0; u<ulen; u++)
		{
		if(pfilter[u]==1)
			pfilter[u]=0;
		}
	}
TCHAR ppath[MAX_PATH];
StringCopy(ppath, MAX_PATH, hpath->Begin());
OPENFILENAME ofn;
ZeroMemory(&ofn, sizeof(OPENFILENAME));
ofn.lStructSize=sizeof(OPENFILENAME);
ofn.Flags=uflags;
ofn.hInstance=GetModuleHandle(NULL);
ofn.hwndOwner=hwnd;
ofn.lpstrTitle=htitle->Begin();
ofn.lpstrFile=ppath;
ofn.nMaxFile=MAX_PATH;
ofn.lpstrFilter=pfilter;
if(!GetSaveFileName(&ofn))
	return nullptr;
return new String(ppath);
}

}}
