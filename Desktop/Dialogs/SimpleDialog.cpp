//==================
// SimpleDialog.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "SimpleDialog.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

INT SimpleDialog::Show(Handle<Window> hparent, Handle<String> htitle, Handle<String> hmsg, UINT ubtns, HRESULT hr)
{
HWND hwnd=hparent? hparent->GetHandle(): NULL;
if(hr!=S_OK)
	{
	TCHAR psys[128];
	if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), psys, 128, 0))
		{
		if(hmsg)
			{
			hmsg=new String("%s\n\n%s", hmsg->Begin(), psys);
			}
		else
			{
			hmsg=psys;
			}
		}
	}
return MessageBox(hwnd, hmsg->Begin(), htitle->Begin(), ubtns);
}

}}
