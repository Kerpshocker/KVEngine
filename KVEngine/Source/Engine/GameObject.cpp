#include "GameObject.h"

using namespace DirectX;

namespace KVE
{
	namespace GameComponents
	{
		GameObject::GameObject( void )
		{

		}

		GameObject::GameObject( Graphics::Mesh gMesh, Graphics::Material gMaterial )
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

		DirectX::XMFLOAT3 GameObject::getPosition( void )
		{
			return position;
		}

		DirectX::XMFLOAT3 GameObject::getRotation( void )
		{
			return rotation;
		}

		DirectX::XMFLOAT3 GameObject::getScale( void )
		{
			return scale;
		}

		void GameObject::setPostion( DirectX::XMFLOAT3 vPosition )
		{
			position = vPosition;
		}

		void GameObject::setRotation( DirectX::XMFLOAT3 vRotation )
		{
			rotation = vRotation;
		}

		void GameObject::setScale( DirectX::XMFLOAT3 vScale )
		{
			scale = vScale;
		}

		void GameObject::saveMesh( Graphics::Mesh gMesh )
		{
			mesh = gMesh;
		}

		void GameObject::saveMaterial( Graphics::Material gMaterial )
		{
			material = gMaterial;
		}
	}
}