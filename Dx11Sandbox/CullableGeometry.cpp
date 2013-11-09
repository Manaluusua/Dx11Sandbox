#include "CullableGeometry.h"

#include "CullInfoManager.h"
#include "RenderObjectManager.h"



namespace Dx11Sandbox
{
	CullableGeometry::CullableGeometry(RenderObjectManager* mngr)
		:m_cullingInformation(0),
		m_mngr(mngr)
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
			if(m_cullingInformation == 0){
				m_cullingInformation = m_mngr->m_cullingInfoManager.allocateDynamic();
				(*m_cullingInformation)->object = this;
			}
		}else{
			if(m_cullingInformation != 0){
				m_mngr->m_cullingInfoManager.deallocateDynamic(m_cullingInformation);
				m_cullingInformation = 0;
			}
		}
	}

	void CullableGeometry::destroy()
	{
		m_mngr->DestroyRenderObject(this);
	}

	void CullableGeometry::setBoundingSphere(const D3DXVECTOR4& bounds)
	{
		(*m_cullingInformation)->boundingSphere = bounds;
	}

    void CullableGeometry::setRenderMask(RenderMask mask)
	{
		(*m_cullingInformation)->renderMask = mask;
	}
	void CullableGeometry::setRenderQueue(RenderQueueID queue)
	{
		m_renderQueue = queue;
	}
	
	

}