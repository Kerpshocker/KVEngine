#include <d3d11.h>

namespace KVE
{
    struct MaterialParams
    {
        ID3D11ShaderResourceView *srv;
        ID3D11SamplerState *ss;
    };

    class Material
    {
    public:
        MaterialParams m_MaterialParams;

    public:
        Material( void );
        Material( const MaterialParams& materialParams );
        ~Material();

    };
}