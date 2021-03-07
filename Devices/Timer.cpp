//===========
// Timer.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Timer.h"


//===========
// Namespace
//===========

namespace Devices {


//==================
// Con-/Destructors
//==================

Timer::Timer():
pTimer(nullptr)
{}

Timer::~Timer()
{
Stop();
}


//========
// Common
//========

VOID Timer::StartOnce(UINT uus)
{
Stop();
hCallback=this;
if(!pTimer)
	pTimer=CreateThreadpoolTimer(TimerProc, this, nullptr);
UINT64 utime=-((UINT64)uus/100);
FILETIME t;
t.dwLowDateTime=(UINT)utime;
t.dwHighDateTime=(UINT)(utime>>32);
SetThreadpoolTimer(pTimer, &t, 0, 0);
}

VOID Timer::StartPeriodic(UINT uus)
{
Stop();
hCallback=this;
if(!pTimer)
	pTimer=CreateThreadpoolTimer(TimerProc, this, nullptr);
SetThreadpoolTimer(pTimer, nullptr, uus/1000, 0);
}

VOID Timer::Stop()
{
if(!pTimer)
	return;
SetThreadpoolTimer(pTimer, nullptr, 0, 0);
WaitForThreadpoolTimerCallbacks(pTimer, false);
CloseThreadpoolTimer(pTimer);
hCallback=nullptr;
pTimer=nullptr;
}


//================
// Common Private
//================

VOID CALLBACK Timer::TimerProc(PTP_CALLBACK_INSTANCE pinst, VOID* pparam, PTP_TIMER ptptimer)
{
auto ptimer=(Timer*)pparam;
ptimer->Triggered(ptimer);
}

}