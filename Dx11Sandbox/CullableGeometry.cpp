#include "CullableGeometry.h"

#include "CullableObjectManager.h"




namespace Dx11Sandbox
{
	CullableGeometry::CullableGeometry(CullableGeometryManager* mngr)
		:m_bounds(-1.f, -1.f,-1.f, -1.f),	
		m_cullingInformation(0),
		m_mngr(mngr),
		m_renderQueue( 0 ),
		m_renderLayer( RENDERLAYER_DEFAULT_OPAQUE )	
	{
		setVisible(true);
	}


	CullableGeometry::~CullableGeometry(void)
	{
		setVisible(false);
	}

	CullableType CullableGeometry::GetCullableType() const
	{
		return CULLABLE_GEOMETRY;
	}

	void CullableGeometry::setVisible(bool value){
		if(value)
		{
			acquireCullData();
		}else{
			returnCullData();
		}
	}

	void CullableGeometry::acquireCullData()
	{
		if(m_cullingInformation == 0){
			m_cullingInformation = m_mngr->GetCullDataAllocator(m_renderLayer)->allocateDynamic();
			(*m_cullingInformation)->object = this;
			updateCullData();
		}
	}
	void CullableGeometry::returnCullData()
	{
		if(m_cullingInformation != 0){
			m_mngr->GetCullDataAllocator(m_renderLayer)->deallocateDynamic(m_cullingInformation);
			m_cullingInformation = 0;
		}
	}

	void CullableGeometry::updateCullData()
	{
		if(m_cullingInformation == 0) return;
		(*m_cullingInformation)->boundingSphere = m_bounds;
	}

	void CullableGeometry::destroy()
	{
		m_mngr->destroy(this);
	}

	void CullableGeometry::setBoundingSphere(const D3DXVECTOR4& bounds)
	{
		m_bounds = bounds;
		updateCullData();
	}

    void CullableGeometry::setRenderMask(RenderLayer mask)
	{	
		if(mask == m_renderLayer) return;

		bool reAcquireCullData = m_cullingInformation == 0 ? false : true;

		if(reAcquireCullData)
		{
			returnCullData();
		}

		m_renderLayer = mask;
		
		if(reAcquireCullData)
		{
			acquireCullData();
		}
		
		
	}
	void CullableGeometry::setRenderQueue(RenderQueueID queue)
	{
		m_renderQueue = queue;
	}
	
	

}