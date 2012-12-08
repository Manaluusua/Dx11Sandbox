#ifndef DX11SANDBOX_TERRAINRENDERER_H
#define DX11SANDBOX_TERRAINRENDERER_H
#include "renderer.h"
#include "RCObjectPtr.h"
#include "Mesh.h"
#include <DXGIFormat.h>
#include <vector>

namespace Dx11Sandbox
{

    class Mesh;

    class TerrainRenderer :
        public Renderer
    {
    public:
        TerrainRenderer(void);
        virtual ~TerrainRenderer(void);
        void render(std::vector<CullInfo*>& objects, RenderContext* state,  Camera* camera);

        void reallocateIndexBuffer( RenderContext* state, unsigned int indexCount );

        void setFormatToUse( DXGI_FORMAT format );
        void setCacheIncreaseRatio( float ratio );
    private:
        Mesh m_cachedMesh;
        std::vector<BYTE> m_cachedShadowBuffer;
        float m_cacheIncreaseRatio;
        DXGI_FORMAT m_formatToUse;

        unsigned int getTotalIndexCount( std::vector<CullInfo*>& objects ); 
            
    };

    inline void TerrainRenderer::setCacheIncreaseRatio( float ratio )
    {
        if( ratio >= 0)  
            m_cacheIncreaseRatio = ratio;
    }

    inline void TerrainRenderer::setFormatToUse( DXGI_FORMAT format )
    {
        m_formatToUse = format;
    }
}

#endif