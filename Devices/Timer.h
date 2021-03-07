//=========
// Timer.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Devices {


//=======
// Timer
//=======

class Timer: public Object
{
public:
	// Con-/Destructors
	Timer();
	~Timer();

	// Common
	BOOL IsStarted()const { return pTimer!=nullptr; }
	VOID StartOnce(UINT MicroSeconds);
	VOID StartPeriodic(UINT MicroSeconds);
	VOID Stop();
	Event<Timer> Triggered;

private:
	// Common
	static VOID CALLBACK TimerProc(PTP_CALLBACK_INSTANCE Instance, VOID* Param, PTP_TIMER Timer);
	Handle<Timer> hCallback;
	PTP_TIMER pTimer;
};

}