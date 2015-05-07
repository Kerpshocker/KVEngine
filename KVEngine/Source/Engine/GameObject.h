#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <DirectXMath.h>
#include "DataTypes.h"
#include "Resources.h"

namespace KVE
{
	namespace GameComponents
	{
		class GameObject
		{
		public:
			Graphics::Mesh			mesh;
			Graphics::Material		material;

		public:
			GameObject( void );
			GameObject( Graphics::Mesh mesh, Graphics::Material material );
			~GameObject();

			void transformWorldMatrix( float deltaTime );

			DirectX::XMFLOAT4X4 getWorldMatrix( void );
			DirectX::XMFLOAT3	getPosition( void );
			DirectX::XMFLOAT3	getRotation( void );
			DirectX::XMFLOAT3	getScale( void );

			void setPostion( DirectX::XMFLOAT3 position );
			void setRotation( DirectX::XMFLOAT3 rotation );
			void setScale( DirectX::XMFLOAT3 scale );

			void saveMesh( Graphics::Mesh mesh );
			void saveMaterial( Graphics::Material material );

		protected:
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT3	position;
			DirectX::XMFLOAT3	rotation;
			DirectX::XMFLOAT3	scale;
		};
	}
}

#endif