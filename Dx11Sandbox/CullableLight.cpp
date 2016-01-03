#include "CullableLight.h"

#include "CullableObjectManager.h"
#include "RenderPrimitiveGatherer.h"
#include <climits>

namespace Dx11Sandbox
{
	CullableLight::CullableLight(CullableObjectManager* mngr)
		:m_bounds(-1.f, -1.f,-1.f, -1.f),	
		m_cullingInformation(0),
		m_mngr(mngr),
		m_lightId(0)
	{
		setVisible(true);
	}


	CullableLight::~CullableLight(void)
	{
		setVisible(false);
	}

	uint32_t CullableLight::getLightId()
	{
		return m_lightId;

	}


	void CullableLight::setLightId(uint32_t id)
	{
		m_lightId = id;
	}
	void CullableLight::passedCulling(RenderPrimitiveGatherer *gatherer)
	{
		gatherer->addLight(this);
	}

	void CullableLight::setVisible(bool value)
	{
		if(value)
		{
			if(m_cullingInformation == 0)
			{
				m_cullingInformation = m_mngr->GetCullDataAllocator(RENDERLAYER_LIGHTPASS)->allocateDynamic();

				CullData* data = (*m_cullingInformation);
				data->object = this;
				data->boundingSphere = m_bounds;
			}
			
	
		}else{
			if(m_cullingInformation != 0)
			{
				m_mngr->GetCullDataAllocator(RENDERLAYER_LIGHTPASS)->deallocateDynamic(m_cullingInformation);
				m_cullingInformation = 0;
			}

		}
		
		
	
	}

	void CullableLight::calculateBoundsForLight(Vec4& bounds) const
	{
		if (m_lightDefinition.lightType == DIRECTIONAL)
		{

			bounds.x = 0.f;
			bounds.y = 0.f;
			bounds.z = 0.f;
			bounds.w = FLT_MAX;
		}
		else if (m_lightDefinition.lightType == OMNI)
		{
			bounds = m_lightDefinition.posRad;
		}
		else{
			//spot. Now just use way too big bounds, optimize later
			bounds = m_lightDefinition.posRad;
		}



	}

	void CullableLight::lightParametersChanged()
	{
		Vec4 newBounds;
		calculateBoundsForLight(newBounds);
		if (newBounds == m_bounds) return;
		m_bounds = newBounds;
		if (m_cullingInformation)
		{
			(*m_cullingInformation)->boundingSphere = m_bounds;
		}
	}


	void CullableLight::destroy()
	{
		m_mngr->destroy(this);
	}

};
