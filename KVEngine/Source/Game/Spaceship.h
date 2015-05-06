#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <DirectXMath.h>
#include "Camera.h"

class Spaceship
{
public:
	Spaceship( void );
	~Spaceship( void );

	void update( float dt );
	void mountCamera( KVE::Graphics::Camera* const camera );

private:
	DirectX::XMVECTOR m_Position;
	DirectX::XMVECTOR m_Rotation;

	float m_Accel;
	float m_YawAccel;
	float m_PitchAccel;
	float m_RollAccel;

};

#endif