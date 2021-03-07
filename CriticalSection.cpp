//=====================
// CriticalSection.cpp
//=====================

#include "pch.h"


//=======
// Using
//=======

#include "CriticalSection.h"


//========
// Common
//========

BOOL CriticalSection::IsLocked()
{
if(TryLock())
	{
	Unlock();
	return false;
	}
return true;
}
