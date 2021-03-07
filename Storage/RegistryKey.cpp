//=================
// RegistryKey.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "RegistryKey.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

RegistryKey::RegistryKey(HKEY hparent, Handle<String> hname, BOOL bcreate):
hKey(NULL)
{
if(!OpenKey(hparent, hname, bcreate))
	{
	if(bcreate)
		throw ERROR_UNIDENTIFIED_ERROR;
	}
}

RegistryKey::~RegistryKey()
{
if(hKey)
	RegCloseKey(hKey);
}


//========
// Common
//========

INT RegistryKey::DecInt(Handle<String> hname)
{
INT i=GetInt(hname);
i--;
SetInt(hname, i);
return i;
}

BOOL RegistryKey::DeleteKey(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
if(RegDeleteTree(hKey, hname->Begin()))
	return false;
return true;
}

BOOL RegistryKey::DeleteValue(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
if(RegDeleteValue(hKey, hname->Begin()))
	return false;
return true;
}

Handle<Buffer> RegistryKey::GetData(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
DWORD usize=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, nullptr, &usize))
	return nullptr;
Handle<Buffer> hbuf=new Buffer(usize);
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, hbuf->Begin(), &usize))
	return nullptr;
return hbuf;
}

INT RegistryKey::GetInt(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
DWORD dwsize=sizeof(DWORD);
INT i=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, (BYTE*)&i, &dwsize))
	throw E_INVALIDARG;
return i;
}

INT RegistryKey::GetInt(Handle<String> hname, INT idefault)
{
ASSERT(hname);
if(!hKey)
	return idefault;
DWORD dwsize=sizeof(DWORD);
INT i=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, (BYTE*)&i, &dwsize))
	return idefault;
return i;
}

UINT RegistryKey::GetKeyCount()
{
if(!hKey)
	throw E_NOT_VALID_STATE;
DWORD dwcount=0;
RegQueryInfoKeyW(hKey, nullptr, nullptr, nullptr, &dwcount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
return dwcount;
}

Handle<String> RegistryKey::GetKeyName(UINT uid)
{
WCHAR pstr[MAX_PATH];
if(RegEnumKeyW(hKey, uid, pstr, MAX_PATH))
	return nullptr;
return new String(pstr);
}

UINT RegistryKey::GetSize(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
DWORD dwsize=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, nullptr, &dwsize))
	throw E_INVALIDARG;
return dwsize;
}

Handle<String> RegistryKey::GetString(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
DWORD dwtype=0;
DWORD usize=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, &dwtype, nullptr, &usize))
	throw E_INVALIDARG;
if(dwtype!=REG_SZ)
	throw E_INVALIDARG;
UINT ulen=usize/sizeof(WCHAR);
if(!ulen)
	return new String("");
Handle<String> hstr=new String(ulen+1, nullptr);
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, (BYTE*)hstr->Begin(), &usize))
	return nullptr;
return hstr;
}

BOOL RegistryKey::HasBool(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	return false;
DWORD dwtype=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, &dwtype, nullptr, nullptr))
	return false;
if(dwtype!=REG_DWORD)
	return false;
DWORD dwsize=sizeof(DWORD);
INT i=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, (BYTE*)&i, &dwsize))
	return false;
if(i<0||i>1)
	return false;
return true;
}

BOOL RegistryKey::HasInt(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	return false;
DWORD dwtype=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, &dwtype, nullptr, nullptr))
	return false;
if(dwtype!=REG_DWORD)
	return false;
return true;
}

BOOL RegistryKey::HasString(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	return false;
DWORD dwtype=0;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, &dwtype, nullptr, nullptr))
	return false;
return dwtype==REG_SZ;
}

BOOL RegistryKey::HasValue(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	return false;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, nullptr, nullptr))
	return false;
return true;
}

INT RegistryKey::IncInt(Handle<String> hname)
{
INT i=GetInt(hname);
i++;
SetInt(hname, i);
return i;
}

BOOL RegistryKey::KeyExists(Handle<String> hname)
{
ASSERT(hname);
if(!hKey)
	return false;
if(RegQueryValueEx(hKey, hname->Begin(), nullptr, nullptr, nullptr, nullptr))
	return false;
return true;
}

BOOL RegistryKey::OpenKey(HKEY hparent, Handle<String> hname, BOOL bcreate)
{
ASSERT(hname);
if(hKey)
	RegCloseKey(hKey);
hKey=0;
LSTATUS status=RegOpenKey(hparent, hname->Begin(), &hKey);
if(status)
	{
	if(!bcreate)
		return false;
	if(RegCreateKey(hparent, hname->Begin(), &hKey))
		throw ERROR_UNIDENTIFIED_ERROR;
	}
return true;
}

VOID RegistryKey::SetData(Handle<String> hname, VOID const* pbuf, UINT usize)
{
ASSERT(hname&&pbuf&&usize);
if(!hKey)
	throw E_NOT_VALID_STATE;
if(RegSetValueEx(hKey, hname->Begin(), 0, REG_BINARY, (const BYTE*)pbuf, usize))
	throw ERROR_UNIDENTIFIED_ERROR;
}

VOID RegistryKey::SetInt(Handle<String> hname, INT i)
{
ASSERT(hname);
if(!hKey)
	throw E_NOT_VALID_STATE;
if(RegSetValueEx(hKey, hname->Begin(), 0, REG_DWORD, (BYTE*)&i, sizeof(DWORD)))
	throw ERROR_UNIDENTIFIED_ERROR;
}

VOID RegistryKey::SetString(Handle<String> hname, Handle<String> hstr)
{
ASSERT(hname&&hstr);
if(!hKey)
	throw E_NOT_VALID_STATE;
#ifdef _UNICODE
LPCWSTR pstr=hstr->Begin();
UINT ulen=hstr->GetLength();
#else
WCHAR pstr[1024];
UINT ulen=StringCopy(pstr, 1024, hstr->Begin());
#endif
UINT usize=(ulen+1)*sizeof(WCHAR);
if(RegSetValueEx(hKey, hname->Begin(), 0, REG_SZ, (BYTE*)pstr, usize))
	throw ERROR_UNIDENTIFIED_ERROR;
}

BOOL RegistryKey::SwitchBool(Handle<String> hname)
{
BOOL b=GetInt(hname);
b=!b;
SetInt(hname, b);
return b;
}

}
