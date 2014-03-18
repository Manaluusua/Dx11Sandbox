#include "BasicMaterialPropertiesSetter.h"
#include "Material.h"
#include "Shader.h"
#include "RenderCamera.h"
#include "EnvironmentInfo.h"
#include "RenderData.h"
#include "Light.h"
#include "d3dx11effect.h"
namespace Dx11Sandbox
{
	BasicMaterialPropertiesSetter::BasicMaterialPropertiesSetter(void)
		:m_cam(0),
		m_lights(0)
	{
	}


	BasicMaterialPropertiesSetter::~BasicMaterialPropertiesSetter(void)
	{
	}

	void BasicMaterialPropertiesSetter::setCurrentCamera(Camera* cam)
	{
		m_cam = cam;
	}

	void BasicMaterialPropertiesSetter::setLights(std::vector<Light*>* lights)
	{
		m_lights = lights;
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

	void BasicMaterialPropertiesSetter::setSceneInfoUniforms(RenderData* object, Material* mat)
	{
		const D3DXMATRIX *view = m_cam->getViewMatrix();
		const D3DXMATRIX *proj = m_cam->getProjectionMatrix();
		const D3DXMATRIX& world = object->getWorldMatrix();
		D3DXMATRIX worldviewProj = world * (*view) * (*proj);

		D3DXVECTOR3 transl = -(m_cam->getTranslation());
		D3DXVECTOR4 camPos(transl.x, transl.y, transl.z, 1);
		const D3DXVECTOR4& clip = m_cam->getClipPlane();

		ID3DX11Effect* effect = mat->getShader()->getEffect();
		ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("basicSceneInfo");
		if (buffer->IsValid())
		{

			ID3DX11EffectMatrixVariable* mat = buffer->GetMemberByName("worldviewProj")->AsMatrix();
			mat->SetMatrix((float*)&worldviewProj);
			mat = buffer->GetMemberByName("world")->AsMatrix();
			mat->SetMatrix((float*)&world);
			buffer->GetMemberByName("camPos")->AsVector()->SetFloatVector((float*)&camPos);
			buffer->GetMemberByName("clipPlane")->AsVector()->SetFloatVector((float*)&clip);
			buffer->GetMemberByName("time")->AsScalar()->SetFloat((float)EnvironmentInfo::getTime());


		}
	}
	void BasicMaterialPropertiesSetter::setLightUniforms(Material* mat)
	{
		//temp
		Light& light = getLight();




		ID3DX11Effect* effect = mat->getShader()->getEffect();
		ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("objectInfo");
		if (buffer->IsValid())
		{
			buffer->GetMemberByName("sunDirection")->AsVector()->SetFloatVector((float*)&light.getDirection());
			buffer->GetMemberByName("sunColor")->AsVector()->SetFloatVector((float*)&light.getColor());
		}

	}

	void BasicMaterialPropertiesSetter::setShaderUniforms(RenderData* object, Material* mat, bool updateSceneUniforms, bool updateLightUniforms)
	{

		if (!updateSceneUniforms && !updateLightUniforms) return;

		//if no camera is set, return
		if(m_cam == 0) return;

		if (updateSceneUniforms){
			setSceneInfoUniforms(object, mat);
		}

		if (updateLightUniforms){
			setLightUniforms(mat);
		}

	}
}