#pragma once
#include <atlbase.h>

static void CALLBACK TimerProc(void*, BOOLEAN);

///////////////////////////////////////////////////////////////////////////////
//
// class CSCTimer
//
class CSCTimer
{
public:
    CSCTimer()
    {
        m_hTimer = NULL;
        m_mutexCount = 0;
    }

    virtual ~CSCTimer()
    {
        Stop();
    }

    bool Start(unsigned int interval,   // interval in ms
               bool immediately = false,// true to call first event immediately
               bool once = false)       // true to call timed event only once
    {
        if( m_hTimer )
        {
            return false;
        }

        SetCount(0);

        BOOL success = CreateTimerQueueTimer( &m_hTimer,
                                              NULL,
                                              TimerProc,
                                              this,
                                              immediately ? 0 : interval,
                                              once ? 0 : interval,
                                              WT_EXECUTEINTIMERTHREAD);

        return( success != 0 );
    }

    void Stop(HANDLE hCompletionEvent = NULL)
    {
        DeleteTimerQueueTimer( NULL, m_hTimer, hCompletionEvent);
        m_hTimer = NULL ;
    }

	bool Change(unsigned int interval,		// interval in ms
				bool immediately = false,	// true to call first event immediately
				bool once = false)			// true to call timed event only once
	{
		if (!m_hTimer)
		{
			return false;
		}

		BOOL success = ChangeTimerQueueTimer(
			NULL,
			m_hTimer,
			immediately ? 0 : interval,
			once ? 0 : interval);

		return(success != 0);
	}

    virtual void OnTimedEvent()
    {
        // Override in derived class
    }

    void SetCount(int value)
    {
        InterlockedExchange( &m_mutexCount, value );
    }

    int GetCount()
    {
        return InterlockedExchangeAdd( &m_mutexCount, 0 );
    }

private:
    HANDLE m_hTimer;
    long m_mutexCount;
};

///////////////////////////////////////////////////////////////////////////////
//
// TimerProc
//
void CALLBACK TimerProc(void* param, BOOLEAN timerCalled)
{
    CSCTimer* timer = static_cast<CSCTimer*>(param);
    timer->SetCount( timer->GetCount()+1 );
    timer->OnTimedEvent();
};

///////////////////////////////////////////////////////////////////////////////
//
// template class TTimer
//
template <class T> class TTimer : public CSCTimer
{
public:
    typedef private void (T::*TimedFunction)(void);

    TTimer()
    {
        m_pTimedFunction = NULL;
        m_pClass = NULL;
    }

    void SetTimedEvent(T *pClass, TimedFunction pFunc)
    {
        m_pClass         = pClass;
        m_pTimedFunction = pFunc;
    }

protected:
    void OnTimedEvent()  
    {
        if (m_pTimedFunction && m_pClass)
        {
            (m_pClass->*m_pTimedFunction)();
        }
    }

private:
    T *m_pClass;
    TimedFunction m_pTimedFunction;
};
