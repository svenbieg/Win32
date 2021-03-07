//===================
// CriticalSection.h
//===================

#pragma once


//==================
// Critical Section
//==================

class CriticalSection
{
public:
	// Con-/Destructors
	CriticalSection() { InitializeCriticalSection(&cCriticalSection); }
	~CriticalSection() { DeleteCriticalSection(&cCriticalSection); }

	// Common
	BOOL IsLocked();
	inline VOID Lock() { EnterCriticalSection(&cCriticalSection); }
	inline BOOL TryLock() { return TryEnterCriticalSection(&cCriticalSection); }
	inline VOID Unlock() { LeaveCriticalSection(&cCriticalSection); }

private:
	// Common
	CRITICAL_SECTION cCriticalSection;
};
