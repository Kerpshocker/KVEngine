#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <DirectXMath.h>
#include "DataTypes.h"
#include "Resources.h"

namespace KVE
{
	namespace GameComponents
	{
		struct Vector
		{
			f32					x;
			f32					y;
			f32					z;
		};

		class GameObject
		{
		public:
			Graphics::Mesh				mesh;
			Graphics::Material			material;

		public:
			GameObject( void );
			GameObject( Graphics::Mesh mesh, Graphics::Material material );
			~GameObject();

			void transformWorldMatrix( float deltaTime );

			DirectX::XMFLOAT4X4 getWorldMatrix( void );
			Vector getPosition( void );
			Vector getRotation( void );
			Vector getScale( void );

			void setPostion( Vector position );
			void setRotation( Vector rotation );
			void setScale( Vector scale );

			void saveMesh( Graphics::Mesh mesh );
			void saveMaterial( Graphics::Material material );

		private:
			DirectX::XMFLOAT4X4 world;
			Vector				position;
			Vector				rotation;
			Vector				scale;
		};
	}
}

#endif