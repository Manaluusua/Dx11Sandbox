#include "CullableLight.h"

#include "CullableObjectManager.h"
#include "RenderPrimitiveGatherer.h"
#include <climits>

namespace Dx11Sandbox
{
	CullableLight::CullableLight(CullableLightManager* mngr)
		:m_bounds(-1.f, -1.f,-1.f, -1.f),	
		m_cullingInformation(0),
		m_mngr(mngr)
	{
		setVisible(true);
	}


	CullableLight::~CullableLight(void)
	{
		setVisible(false);
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

	void CullableLight::setLightType(LightType type)
	{
		if(type == m_lightType) return;
		
		if(type == DIRECTIONAL)
		{
			m_bounds.w = FLT_MAX;
		}else
		{
			m_bounds.w = m_params.w;
		}
		

		m_lightType = type;

		if(m_cullingInformation)
		{
			(*m_cullingInformation)->boundingSphere = m_bounds;
		}
	}

	void CullableLight::setLightParameters(const D3DXVECTOR4& params)
	{
		if(params == m_params) return;
		
		m_bounds = params;
		Light::setLightParameters(params);

		if(m_lightType == DIRECTIONAL)
		{
			m_bounds.w = FLT_MAX;
		}

		if(m_cullingInformation)
		{
			(*m_cullingInformation)->boundingSphere = m_bounds;
		}
	}

	void CullableLight::destroy()
	{
		m_mngr->destroy(this);
	}

};
