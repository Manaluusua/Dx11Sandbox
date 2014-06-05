#include "CullableGeometry.h"
#include "RenderPrimitiveGatherer.h"
#include "CullData.h"
#include "CullableObjectManager.h"




namespace Dx11Sandbox
{
	CullableGeometry::CullableGeometry(CullableGeometryManager* mngr)
		:m_bounds(-1.f, -1.f,-1.f, -1.f),	
		m_cullingInformation(0),
		m_mngr(mngr)	
	{
		setVisible(true);
	}


	CullableGeometry::~CullableGeometry(void)
	{
		setVisible(false);
	}



	void CullableGeometry::passedCulling(RenderPrimitiveGatherer *gatherer)
	{
		gatherer->addGeometry(this);
	}

	void CullableGeometry::setVisible(bool value){
		if(value)
		{
			acquireCullData();
		}else{
			returnCullData();
		}
	}

	void CullableGeometry::setWorldMatrix(const D3DXMATRIX& matrix)
	{
		m_renderData.setWorldMatrix(matrix);
		updateCullData();

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

		//calculate world space bounds
		D3DXVECTOR3 center = m_bounds;
		D3DXVECTOR4 boundsWorld;
		D3DXVec3Transform(&boundsWorld, &center, &m_renderData.getWorldMatrix());
		boundsWorld.w = calculateWorldSpaceRadius(m_bounds.w) * 0.9f;;

		(*m_cullingInformation)->boundingSphere = boundsWorld;
	}


	float CullableGeometry::calculateWorldSpaceRadius(float localRadius)
	{
		if (localRadius <= 0.f) return localRadius;
		float lenSqr = 0.f;
		D3DXVECTOR3 local;
		D3DXVECTOR3 world;

		for (int i = 0; i < 3; ++i){
			local.x = i == 0 ? localRadius : 0.f;
			local.y = i == 1 ? localRadius : 0.f;
			local.z = i == 2 ? localRadius : 0.f;

			D3DXVec3TransformNormal(&world, &local, &m_renderData.getWorldMatrix());

			float lqsr = D3DXVec3LengthSq(&world);
			if (lqsr <= lenSqr) continue;
			lenSqr = lqsr;

		}
		return sqrtf(lenSqr);
		
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

		BasicGeometry::setRenderMask(mask);
		
		if(reAcquireCullData)
		{
			acquireCullData();
		}
		
		
	}

	

}