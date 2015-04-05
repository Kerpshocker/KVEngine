//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include "DataTypes.h"

class GameTimer
{
public:
	GameTimer( void );

	f32 totalTime( void ) const;
	f32 deltaTime( void ) const;
	f32 elapsedTime( void ) const;

	void start( void );
	void stop( void );
	void tick( void );
	void reset( void );

private:
	f64 mSecondsPerCount;
	f64 mDeltaTime;

	i64 mBaseTime;
	i64 mPausedTime;
	i64 mStopTime;
	i64 mPrevTime;
	i64 mCurrTime;

	bool mStopped;

};

#endif