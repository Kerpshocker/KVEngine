#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <DirectXMath.h>
#include "DataTypes.h"
#include "Mesh.h"
#include "Material.h"

namespace KVE
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
		Mesh				mesh;
		Material			material;

	public:
		GameObject( void );
		GameObject( Mesh mesh, Material material );
		~GameObject();

		void transformWorldMatrix( float deltaTime );

		DirectX::XMFLOAT4X4 getWorldMatrix( void );
		Vector getPosition( void );
		Vector getRotation( void );
		Vector getScale( void );

		void setPostion( Vector position );
		void setRotation( Vector rotation );
		void setScale( Vector scale );

		void saveMesh( Mesh mesh );
		void saveMaterial( Material material );

	private:
		DirectX::XMFLOAT4X4 world;
		Vector				position;
		Vector				rotation;
		Vector				scale;
	};
}

#endif