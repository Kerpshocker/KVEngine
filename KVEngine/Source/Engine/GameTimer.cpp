//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "GameTimer.h"
#include <Windows.h>

GameTimer::GameTimer( void ) : mSecondsPerCount( 0.0 ), mDeltaTime( -1.0 ), mBaseTime( 0 ),
	mPausedTime( 0 ), mPrevTime( 0 ), mCurrTime( 0 ), mStopped( false )
{
	i64 countsPerSec;
	QueryPerformanceFrequency( (LARGE_INTEGER*) &countsPerSec );
	mSecondsPerCount = 1.0 / (f64) countsPerSec;
}

f32 GameTimer::totalTime( void ) const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if ( mStopped )
	{
		return (f32) ( ( ( mStopTime - mPausedTime ) - mBaseTime )*mSecondsPerCount );
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return (f32) ( ( ( mCurrTime - mPausedTime ) - mBaseTime )*mSecondsPerCount );
	}
}

f32 GameTimer::deltaTime( void ) const
{
	return (f32) mDeltaTime;
}

void GameTimer::start( void )
{
	i64 startTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &startTime );


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if ( mStopped )
	{
		mPausedTime += ( startTime - mStopTime );

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::stop( void )
{
	if ( !mStopped )
	{
		i64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );

		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::tick( void )
{
	if ( mStopped )
	{
		mDeltaTime = 0.0;
		return;
	}

	i64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = ( mCurrTime - mPrevTime )*mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if ( mDeltaTime < 0.0 )
	{
		mDeltaTime = 0.0;
	}
}

void GameTimer::reset( void )
{
	i64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &currTime );

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}