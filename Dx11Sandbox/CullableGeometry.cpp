#include "CullableGeometry.h"
#include "RenderPrimitiveGatherer.h"
#include "CullData.h"
#include "CullableObjectManager.h"




namespace Dx11Sandbox
{
	CullableGeometry::CullableGeometry(CullableObjectManager* mngr)
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

	void CullableGeometry::setWorldMatrix(const Matrix& matrix)
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
		Vec3 center(m_bounds[0], m_bounds[1], m_bounds[2]);
		Vec4 boundsWorld;
		transformVec3Point(center, m_renderData.getWorldMatrix(), center);
		boundsWorld[3] = calculateWorldSpaceRadius(m_bounds[3]);
		(*m_cullingInformation)->boundingSphere = boundsWorld;
	}


	float CullableGeometry::calculateWorldSpaceRadius(float localRadius)
	{
		if (localRadius <= 0.f) return localRadius;
		float lenSqr = 0.f;
		Vec3 s(localRadius, localRadius, localRadius);
		transformVec3Direction(s, m_renderData.getWorldMatrix(), s);

		return sqrtf(max(max(s[0], s[1]), s[2]));

	}

	void CullableGeometry::destroy()
	{
		m_mngr->destroy(this);
	}

	void CullableGeometry::setBoundingSphere(const Vec4& bounds)
	{
		m_bounds = bounds;
		m_renderData.setWorldBounds(m_bounds);
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