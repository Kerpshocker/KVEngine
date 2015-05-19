#include "Spaceship.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "FirstPersonCamera.h"

#define MAX_ACCEL 3
#define MAX_YAW_ACCEL 1
#define MAX_PITCH_ACCEL 1
#define MAX_ROLL_ACCEL 1

Spaceship::Spaceship( void )
{

}

Spaceship::~Spaceship( void )
{

}

void Spaceship::update( double dt )
{
	const int inputKeyData = Input::GetKeyInputData();

	if ( inputKeyData & Input::KEY_MOVE_FORWARD )
	{
		m_Vel += 0.0025f;
		if ( m_Vel > MAX_ACCEL ) m_Vel = MAX_ACCEL;
	}
	else if ( inputKeyData & Input::KEY_MOVE_BACKWARD )
	{
		m_Vel -= 0.0025f;
		if ( m_Vel < -MAX_ACCEL ) m_Vel = -MAX_ACCEL;
	}
	else
	{
		m_Vel *= 0.99975f;
		if ( abs( m_Vel ) < 0.000001f ) m_Vel = 0.0f;
	}

	if ( inputKeyData & Input::KEY_PITCH_POS )
	{
		m_PitchVel += 0.00025f;
		if ( m_PitchVel > MAX_PITCH_ACCEL ) m_PitchVel = MAX_PITCH_ACCEL;
	}
	else if ( inputKeyData & Input::KEY_PITCH_NEG )
	{
		m_PitchVel -= 0.00025f;
		if ( m_PitchVel < -MAX_PITCH_ACCEL ) m_PitchVel = -MAX_PITCH_ACCEL;
	}
	else
	{
		m_PitchVel *= 0.99975f;
		if ( abs( m_PitchVel ) < 0.000001f ) m_PitchVel = 0.0f;
	}

	if ( inputKeyData & Input::KEY_YAW_POS )
	{
		m_YawVel += 0.00025f;
		if ( m_YawVel > MAX_YAW_ACCEL ) m_YawVel = MAX_YAW_ACCEL;
	}
	else if ( inputKeyData &  Input::KEY_YAW_NEG )
	{
		m_YawVel -= 0.00025f;
		if ( m_YawVel > MAX_YAW_ACCEL ) m_YawVel = MAX_YAW_ACCEL;
	}
	else
	{
		m_YawVel *= 0.99975f;
		if ( abs( m_YawVel ) < 0.000001f ) m_YawVel = 0.0f;
	}

	if ( inputKeyData & Input::KEY_ROLL_POS )
	{
		m_RollVel += 0.00025f;
		if ( m_RollVel > MAX_ROLL_ACCEL ) m_RollVel = MAX_ROLL_ACCEL;
	}
	else if ( inputKeyData & Input::KEY_ROLL_NEG )
	{
		m_RollVel -= 0.00025f;
		if ( m_RollVel < -MAX_ROLL_ACCEL ) m_RollVel = -MAX_ROLL_ACCEL;
	}
	else
	{
		m_RollVel *= 0.99975f;
		if ( abs( m_RollVel ) < 0.000001f ) m_RollVel = 0.0f;
	}

	KVE::GameComponents::FirstPersonCamera* camera = ( KVE::GameComponents::FirstPersonCamera* )KVE::Graphics::CameraManager::Instance().getActiveCamera();

	camera->move( (float)(m_Vel * dt) );
	camera->rotatePitch( (float)(m_PitchVel * dt) );
	camera->rotateYaw( (float)(m_YawVel * dt) );
	camera->rotateRoll( (float)(m_RollVel * dt) );

	// negatives and positives here could result in 0-value, but that possibility
	// is so rare for our purposes we can consider it negligible
	if ( ( m_Vel + m_PitchVel ) + ( m_YawVel + m_RollVel ) )
	{
		KVE::Graphics::CameraManager::Instance().getActiveCamera()->setViewMatrix();
	}
}

void Spaceship::mountCamera( KVE::Graphics::Camera* const camera )
{

}