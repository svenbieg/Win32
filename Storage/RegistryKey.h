//===============
// RegistryKey.h
//===============

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"


//===========
// Namespace
//===========

namespace Storage {


//==============
// Registry-Key
//==============

class RegistryKey: public Object
{
public:
	// Con-/Destructors
	RegistryKey(): hKey(NULL) {}
	RegistryKey(HKEY Key): hKey(Key) {}
	RegistryKey(HKEY Parent, Handle<String> Name): RegistryKey(Parent, Name, false) {}
	RegistryKey(HKEY Parent, Handle<String> Name, BOOL Create);
	~RegistryKey();

	// Common
	INT DecInt(Handle<String> Name);
	BOOL DeleteKey(Handle<String> Name);
	BOOL DeleteValue(Handle<String> Name);
	BOOL GetBool(Handle<String> Name) { return GetInt(Name); }
	BOOL GetBool(Handle<String> Name, BOOL Default) { return GetInt(Name, Default); }
	Handle<Buffer> GetData(Handle<String> Name);
	HKEY GetHandle()const { return hKey; }
	INT GetInt(Handle<String> Name);
	INT GetInt(Handle<String> Name, INT Default);
	UINT GetKeyCount();
	Handle<String> GetKeyName(UINT Id);
	UINT GetSize(Handle<String> Name);
	Handle<String> GetString(Handle<String> Name);
	BOOL HasBool(Handle<String> Name);
	BOOL HasInt(Handle<String> Name);
	BOOL HasString(Handle<String> Name);
	BOOL HasValue(Handle<String> Name);
	INT IncInt(Handle<String> Name);
	BOOL KeyExists(Handle<String> Name);
	BOOL OpenKey(HKEY Parent, Handle<String> Name) { return OpenKey(Parent, Name, false); }
	BOOL OpenKey(HKEY Parent, Handle<String> Name, BOOL Create);
	VOID SetBool(Handle<String> Name, BOOL Value) { SetInt(Name, Value); }
	VOID SetData(Handle<String> Name, VOID const* Buffer, UINT Size);
	VOID SetInt(Handle<String> Name, INT Value);
	VOID SetString(Handle<String> Name, Handle<String> Value);
	BOOL SwitchBool(Handle<String> Name);

private:
	// Common
	HKEY hKey;
};

}
