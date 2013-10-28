#include "TerrainBinHandler.h"


#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Camera.h"
#include "CullInfo.h"
#include "RenderObject.h"
#include "d3dx11effect.h"
#include <D3D11.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{

    TerrainBinHandler::TerrainBinHandler(void)
        :m_cacheIncreaseRatio( 0.5 ),
        m_formatToUse( DXGI_FORMAT_R32_UINT )
    {
		m_cachedRenderData.setMesh(new Mesh);
		m_cachedRenderList.push_back(&m_cachedRenderData);
    }


    TerrainBinHandler::~TerrainBinHandler(void)
    {
    }

    unsigned int TerrainBinHandler::getTotalIndexCount( RenderObject* objects, unsigned int objectCount )
    {
        unsigned int totalIndexCount = 0;
        for( unsigned int i = 0; i < objectCount; ++i )
        {
			RenderObject* object = &objects[i];

			
            IndexBuffer* ib = object->getMesh()->getIndexBuffer();
            if( ib )
            {
               
#if defined( DEBUG )           
                if( ib->getFormat() != m_formatToUse )
                {
                    showErrorDialog( " Tried to use incorrect index format in Terrain RenderBinHandler" );
                }
#endif

                totalIndexCount += ib->getIndexCount();
            }
        }
        return totalIndexCount;
    }

    void TerrainBinHandler::reallocateIndexBuffer(RenderContext* state, unsigned int indexCount )
    {
		m_cachedRenderData.getMesh()->setIndexBuffer( new IndexBuffer( state->getDevice(), 0, m_formatToUse, indexCount, false,D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE ) );
        m_cachedShadowBuffer.resize( (m_formatToUse == DXGI_FORMAT_R16_UINT?2:4) * indexCount );
    }

	void TerrainBinHandler::setupForRendering(RenderObject* objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state)
	{
		 //early out
        if( objectCount == 0 )
        {
            return;
        }


        //make sure we have enough space for the indices
        unsigned int indexCount = getTotalIndexCount( objects, objectCount );

        if( !m_cachedRenderData.getMesh()->getIndexBuffer() ||  m_cachedRenderData.getMesh()->getIndexBuffer()->getIndexCount() < indexCount )
        {
            reallocateIndexBuffer( state, indexCount + static_cast<unsigned int>( static_cast<float>( indexCount) * m_cacheIncreaseRatio ) );
        }
		Mesh* prototypeMesh = objects->getMesh();
		Material* prototypeMaterial = objects->getMaterial();
        IndexBuffer* aggbuffer = m_cachedRenderData.getMesh()->getIndexBuffer();

        //copy indices to one buffer
        int indexOffset = 0;
        int indexSize = (m_formatToUse == DXGI_FORMAT_R16_UINT?2:4);
        int indexesToDraw = 0;
        for( unsigned int i = 0; i < objectCount; ++i )
        {
			IndexBuffer* ib = objects->getMesh()->getIndexBuffer();
            if( ib )
            {
                memcpy( &m_cachedShadowBuffer[ indexOffset ], ib->getShadowBuffer(), ib->getShadowBufferSize() ); 
                indexOffset += ib->getIndexCount() * indexSize;
                indexesToDraw += ib->getIndexCount();
            }
			++objects;
        }

        //cpu -> gpu
        aggbuffer->setDataFromCPUBuffer( state->getImmediateContext(), &m_cachedShadowBuffer[0], indexOffset );
		m_cachedRenderData.getMesh()->setVertexBuffer( prototypeMesh->getVertexBuffer() );

		m_cachedRenderData.setMaterial(prototypeMaterial);
		objectsOut = m_cachedRenderList.data();
		*objectsOutCount = m_cachedRenderList.size();
	}
	/*
    void TerrainBinHandler::render(std::vector<CullInfo*>& objects, RenderContext* state,  Camera* camera)
    {


        //early out
        if( objects.empty() )
        {
            return;
        }


        //make sure we have enough space for the indices
        unsigned int indexCount = getTotalIndexCount( objects );

        if( !m_cachedMesh.getIndexBuffer() ||  m_cachedMesh.getIndexBuffer()->getIndexCount() < indexCount )
        {
            reallocateIndexBuffer( state, indexCount + static_cast<unsigned int>( static_cast<float>( indexCount) * m_cacheIncreaseRatio ) );
        }

        IndexBuffer* aggbuffer = m_cachedMesh.getIndexBuffer();

        //copy indices to one buffer
        int indexOffset = 0;
        int indexSize = (m_formatToUse == DXGI_FORMAT_R16_UINT?2:4);
        int indexesToDraw = 0;
        for( unsigned int i = 0; i < objects.size(); ++i )
        {
			IndexBuffer* ib = objects[i]->object->getMesh()->getIndexBuffer();
            if( ib )
            {
                memcpy( &m_cachedShadowBuffer[ indexOffset ], ib->getShadowBuffer(), ib->getShadowBufferSize() ); 
                indexOffset += ib->getIndexCount() * indexSize;
                indexesToDraw += ib->getIndexCount();
            }
        }

        //cpu -> gpu
        aggbuffer->setDataFromCPUBuffer( state->getImmediateContext(), &m_cachedShadowBuffer[0], indexOffset );

        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        const CullInfo* object = objects[0];

		Mesh* prototypeMesh = object->object->getMesh();
		Material* mat = object->object->getMaterial();
        
        ID3DX11Effect* effect = mat->getEffect();
        ID3D11DeviceContext* context = state->getImmediateContext();


        

        
        
        effect->GetDesc(&effectDesc);
        
        //for now use first tech
        ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(0);
        tech->GetDesc(&techDesc);

        tech->GetPassByIndex(0)->GetDesc(&passDesc);
        

        m_cachedMesh.setVertexBuffer( prototypeMesh->getVertexBuffer() );

        state->bindMesh(&m_cachedMesh);
        state->bindMaterial(mat);

            
        
        

        for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
        {
            tech->GetPassByIndex(passInd)->Apply(0, context );
            if( indexesToDraw > 0)
            {
                context->DrawIndexed(indexesToDraw, 0, 0);
            }
        }
    }
	*/

	
}