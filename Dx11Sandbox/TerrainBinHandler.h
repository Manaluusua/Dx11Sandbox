#ifndef DX11SANDBOX_TerrainBinHandler_H
#define DX11SANDBOX_TerrainBinHandler_H
#include "GeometryBinHandler.h"
#include "RCObjectPtr.h"
#include "RenderData.h"
#include <DXGIFormat.h>
#include <vector>

namespace Dx11Sandbox
{

    class Mesh;

    class TerrainBinHandler :
        public GeometryBinHandler
    {
    public:
        TerrainBinHandler(void);
        virtual ~TerrainBinHandler(void);
		void setupForRendering(RenderData** objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state);

        void reallocateIndexBuffer( RenderContext* state, unsigned int indexCount );

        void setFormatToUse( DXGI_FORMAT format );
        void setCacheIncreaseRatio( float ratio );
    private:
        RenderData m_cachedRenderData;
		std::vector<RenderData*> m_cachedRenderList;
        std::vector<BYTE> m_cachedShadowBuffer;
        float m_cacheIncreaseRatio;
        DXGI_FORMAT m_formatToUse;

        unsigned int getTotalIndexCount( RenderData** objects, unsigned int objectCount ); 
            
    };

    inline void TerrainBinHandler::setCacheIncreaseRatio( float ratio )
    {
        if( ratio >= 0)  
            m_cacheIncreaseRatio = ratio;
    }

    inline void TerrainBinHandler::setFormatToUse( DXGI_FORMAT format )
    {
        m_formatToUse = format;
    }
}

#endif