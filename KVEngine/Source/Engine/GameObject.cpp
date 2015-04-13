#include "GameObject.h"

using namespace DirectX;

namespace KVE
{
	GameObject::GameObject( void )
	{

	}

	GameObject::GameObject( Mesh gMesh, Material gMaterial )
	{
		mesh = gMesh;
		material = gMaterial;
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::transformWorldMatrix( float dt )
	{
		//do some calculations on whichever pos vector based on dt
	}

	XMFLOAT4X4 GameObject::getWorldMatrix( void )
	{
		return world;
	}

	Vector GameObject::getPosition( void )
	{
		return position;
	}

	Vector GameObject::getRotation( void )
	{
		return rotation;
	}

	Vector GameObject::getScale( void )
	{
		return scale;
	}

	void GameObject::setPostion( Vector vPosition )
	{
		position = vPosition;
	}

	void GameObject::setRotation( Vector vRotation )
	{
		rotation = vRotation;
	}

	void GameObject::setScale( Vector vScale )
	{
		scale = vScale;
	}

	void GameObject::saveMesh( Mesh gMesh )
	{
		mesh = gMesh;
	}

	void GameObject::saveMaterial( Material gMaterial )
	{
		material = gMaterial;
	}
}