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

	double m_Vel;
	double m_YawVel;
	double m_PitchVel;
	double m_RollVel;

};

#endif