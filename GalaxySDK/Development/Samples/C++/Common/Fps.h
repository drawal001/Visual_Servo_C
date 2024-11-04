//--------------------------------------------------------------------------------
/**
\file     Fps.h
\brief    CFps类的声明文件
*/
//----------------------------------------------------------------------------------

#ifndef FPS_H
#define FPS_H

#include "StopWatch.h"

//--------------------------------------------------------------------------------
/**
\file     CFps类用于计算帧率。
			
		  CFps是线程安全的。
*/
//----------------------------------------------------------------------------------

class CFps
{
public:
	///构造函数
	CFps(void);

	///析构函数
	~CFps(void);	

    ///获取最近一次的帧率
    double GetFps(void);

    ///获取累积的总帧数
    inline size_t GetTotalFrameCount(void); 

	///增加帧数
	void IncreaseFrameNum(void);

    ///更新帧率
    void UpdateFps(void);

    ///将计时器恢复为初始状态
    inline void Reset(void);

private:
	size_t    m_nFrameCount;        ///< 从上次计算完毕开始累积的帧数

    double    m_dBeginTime;         ///< 第一帧之前的一帧的时间（初始为0）
    double    m_dEndTime;           ///< 最后一帧的时间

	double    m_dFps;               ///< 最近一次的帧率(帧/秒)
	size_t    m_nTotalFrameCount;   ///< 累积的帧数


	CStopWatch m_objStopWatch;            ///< 计时器
};

//----------------------------------------------------------------------------------
/**
\brief  获取累积的总帧数
\param  pFrame  当前帧图像
*/
//----------------------------------------------------------------------------------
inline size_t CFps::GetTotalFrameCount(void) 
{
    return m_nTotalFrameCount;
} 



//----------------------------------------------------------------------------------
/**
\brief  将计时器恢复为初始状态
*/
//----------------------------------------------------------------------------------

inline void CFps::Reset(void)
{
    m_nFrameCount       = 0;
    m_dBeginTime        = 0;
    m_dEndTime          = 0;
    m_nTotalFrameCount  = 0;
    m_dFps              = 0;
}




#endif //FPS_H