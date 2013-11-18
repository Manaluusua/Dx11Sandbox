#include "BasicMaterialPropertiesSetter.h"
#include "Material.h"
#include "RenderCamera.h"
#include "EnvironmentInfo.h"
#include "Light.h"

namespace Dx11Sandbox
{
	BasicMaterialPropertiesSetter::BasicMaterialPropertiesSetter(void)
		:m_cam(0),
		m_previousMaterial(0),
		m_lights(0)
	{
	}


	BasicMaterialPropertiesSetter::~BasicMaterialPropertiesSetter(void)
	{
	}

	void BasicMaterialPropertiesSetter::setCurrentCamera(RenderCamera* cam)
	{
		m_cam = cam;
		m_previousMaterial = 0;
	}

	void BasicMaterialPropertiesSetter::setLights(std::vector<Light*>& lights)
	{
		m_lights = &lights;
	}

	Light& BasicMaterialPropertiesSetter::getLight()
	{
		static Light defaultLight;

		if(m_lights == 0 || m_lights->size() == 0)
		{
			return defaultLight;
		}


		return *(*m_lights)[0];

	}

	void BasicMaterialPropertiesSetter::setShaderProperties(Material* mat)
	{
		if(m_previousMaterial != 0 && m_previousMaterial == mat)return;
		const D3DXMATRIX *view = m_cam->getViewMatrix();
		const D3DXMATRIX *proj = m_cam->getProjectionMatrix();
		D3DXMATRIX viewProj =  (*view) * (*proj);

        //temp
		Light& light = getLight();


		
		D3DXVECTOR3 transl = -(m_cam->getTranslation());
		D3DXVECTOR4 camPos(transl.x, transl.y, transl.z, 1);
		const D3DXVECTOR4& clip = m_cam->getClipPlane();

		ID3DX11Effect* effect =  mat->getEffect();
		ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("sceneInfo");
		if(buffer->IsValid())
		{
			
			ID3DX11EffectMatrixVariable* mat =  buffer->GetMemberByName("viewProj")->AsMatrix();
			mat->SetMatrix((float*)&viewProj);
			buffer->GetMemberByName("sunDirection")->AsVector()->SetFloatVector((float*)&light.getLightParameters());
			buffer->GetMemberByName("sunColor")->AsVector()->SetFloatVector((float*)&light.getColor());
			buffer->GetMemberByName("camPos")->AsVector()->SetFloatVector((float*)&camPos);
			buffer->GetMemberByName("clipPlane")->AsVector()->SetFloatVector((float*)&clip);
			buffer->GetMemberByName("time")->AsScalar()->SetFloat((float)EnvironmentInfo::getTime());
    
    
		}
		m_previousMaterial = mat;
	}
}