#include "RenderObject.h"

#include "CullInfoManager.h"
#include "RenderObjectManager.h"



namespace Dx11Sandbox
{
	RenderObject::RenderObject(void)
		:m_cullingInformation(0),
		m_mngr(0)
	{
		setVisible(true);
	}


	RenderObject::~RenderObject(void)
	{
		setVisible(false);
	}

	void RenderObject::setVisible(bool value){
		if(value)
		{
			if(m_cullingInformation == 0){
				m_cullingInformation = CullInfoManager::singleton()->allocate();
				(*m_cullingInformation)->object = this;
			}
		}else{
			if(m_cullingInformation != 0){
				CullInfoManager::singleton()->deallocate(m_cullingInformation);
				m_cullingInformation = 0;
			}
		}
	}

	void RenderObject::destroy()
	{
		m_mngr->DestroyRenderObject(this);
	}

	void RenderObject::setBoundingSphere(const D3DXVECTOR4& bounds)
	{
		(*m_cullingInformation)->boundingSphere = bounds;
	}

    void RenderObject::setRenderMask(RenderMask mask)
	{
		(*m_cullingInformation)->renderMask = mask;
	}
	void RenderObject::setRenderQueue(RenderQueueID queue)
	{
		m_renderQueue = queue;
	}
	
	

}