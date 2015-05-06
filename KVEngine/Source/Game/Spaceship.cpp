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

void Spaceship::update( float dt )
{
	if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_MOVE_FORWARD ) )
	{
		m_Accel += 0.0025f;
		if ( m_Accel > MAX_ACCEL ) m_Accel = MAX_ACCEL;
	}
	else if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_MOVE_BACKWARD ) )
	{
		m_Accel -= 0.0025f;
		if ( m_Accel < -MAX_ACCEL ) m_Accel = -MAX_ACCEL;
	}
	else
	{
		m_Accel *= 0.99975f;
		if ( abs( m_Accel ) < 0.000001f ) m_Accel = 0.0f;
	}

	if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_PITCH_POS ) )
	{
		m_PitchAccel += 0.00025f;
		if ( m_PitchAccel > MAX_PITCH_ACCEL ) m_PitchAccel = MAX_PITCH_ACCEL;
	}
	else if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_PITCH_NEG ) )
	{
		m_PitchAccel -= 0.00025f;
		if ( m_PitchAccel < -MAX_PITCH_ACCEL ) m_PitchAccel = -MAX_PITCH_ACCEL;
	}
	else
	{
		m_PitchAccel *= 0.99975f;
		if ( abs( m_PitchAccel ) < 0.000001f ) m_PitchAccel = 0.0f;
	}

	if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_YAW_POS ) )
	{
		m_YawAccel += 0.00025f;
		if ( m_YawAccel > MAX_YAW_ACCEL ) m_YawAccel = MAX_YAW_ACCEL;
	}
	else if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_YAW_NEG ) )
	{
		m_YawAccel -= 0.00025f;
		if ( m_YawAccel > MAX_YAW_ACCEL ) m_YawAccel = MAX_YAW_ACCEL;
	}
	else
	{
		m_YawAccel *= 0.99975f;
		if ( abs( m_YawAccel ) < 0.000001f ) m_YawAccel = 0.0f;
	}

	if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_ROLL_POS ) )
	{
		m_RollAccel += 0.00025f;
		if ( m_RollAccel > MAX_ROLL_ACCEL ) m_RollAccel = MAX_ROLL_ACCEL;
	}
	else if ( KVE::Input::IsKeyPressed( KVE::Input::KEY_ROLL_NEG ) )
	{
		m_RollAccel -= 0.00025f;
		if ( m_RollAccel < -MAX_ROLL_ACCEL ) m_RollAccel = -MAX_ROLL_ACCEL;
	}
	else
	{
		m_RollAccel *= 0.99975f;
		if ( abs( m_RollAccel ) < 0.000001f ) m_RollAccel = 0.0f;
	}

	KVE::GameComponents::FirstPersonCamera* camera = ( KVE::GameComponents::FirstPersonCamera* )KVE::Graphics::CameraManager::Instance().getActiveCamera();

	camera->move( m_Accel * dt );
	camera->rotatePitch( m_PitchAccel * dt );
	camera->rotateYaw( m_YawAccel * dt );
	camera->rotateRoll( m_RollAccel * dt );

	//if ( KVE::Input::IsViewChanged() )
	//{
		KVE::Graphics::CameraManager::Instance().getActiveCamera()->setViewMatrix();
	//}
}

void Spaceship::mountCamera( KVE::Graphics::Camera* const camera )
{

}