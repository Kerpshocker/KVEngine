#include "Material.h"

namespace KVE
{
    Material::Material( void )
    {

    }

    Material::Material( const MaterialParams& materialParams )
    {
        m_MaterialParams.ss = materialParams.ss;
        m_MaterialParams.srv = materialParams.srv;
    }

    Material::~Material()
    {
    }
}