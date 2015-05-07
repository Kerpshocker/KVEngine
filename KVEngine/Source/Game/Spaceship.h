#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <DirectXMath.h>
#include "Camera.h"
#include "GameObject.h"

class Spaceship : public KVE::GameComponents::GameObject
{
public:
	Spaceship( void );
	~Spaceship( void );

	void update( float dt );
	void mountCamera( KVE::Graphics::Camera* const camera );

private:

	float m_Accel;
	float m_YawAccel;
	float m_PitchAccel;
	float m_RollAccel;

};

#endif