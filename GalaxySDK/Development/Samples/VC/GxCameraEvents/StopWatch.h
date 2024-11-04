// 时间计数器
class CStopWatch
{	
public:
	
	/// 构造函数	
	CStopWatch()		
	{
		Start();		
	}
	
	/// 开始计数	
	void Start()		
	{		
		QueryPerformanceCounter(&m_StartTime);		
	}
	
	/// 停止计数,s	
	double Stop(bool StartAgain)
		
	{		
		QueryPerformanceCounter(&m_StopTime);
		
		double theElapsedTime = ElapsedTime();
		
		if(StartAgain)
		{
			m_StartTime = m_StopTime; 
		}	
		return theElapsedTime;
		
	}
	
	// 获取时间差	
	double ElapsedTime()
		
	{		
		LARGE_INTEGER timerFrequency;		
		QueryPerformanceFrequency(&timerFrequency);		
		__int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;		
		__int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart;		
		long double timeDifference = (long double) (newTicks - oldTicks);		
		long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) 
			+ (__int64)timerFrequency.LowPart);
		return (double)(timeDifference / ticksPerSecond);		
	}
	
protected:
	
	LARGE_INTEGER m_StartTime;  ///< 开始时间	
	LARGE_INTEGER m_StopTime;   ///< 停止时间	
};
