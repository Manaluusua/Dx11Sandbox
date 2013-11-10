#include "BasicMaterialPropertiesSetter.h"
#include "Material.h"
#include "RenderCamera.h"

namespace Dx11Sandbox
{
	BasicMaterialPropertiesSetter::BasicMaterialPropertiesSetter(void)
	{
	}


	BasicMaterialPropertiesSetter::~BasicMaterialPropertiesSetter(void)
	{
	}

	void BasicMaterialPropertiesSetter::SetCurrentCamera(RenderCamera* cam)
	{
		m_cam = cam;
		m_previousMaterial = 0;
	}


	void BasicMaterialPropertiesSetter::SetShaderProperties(Material* mat)
	{
		if(m_previousMaterial != 0 && m_previousMaterial == mat)return;
		const D3DXMATRIX *view = m_cam->getViewMatrix();
		const D3DXMATRIX *proj = m_cam->getProjectionMatrix();
		D3DXMATRIX viewProj =  (*view) * (*proj);

        //temp
		D3DXVECTOR4 sunDir( 0.f, 0.f, 1.f,0.f );
		D3DXVec4Normalize(&sunDir, &sunDir);
		D3DXVECTOR4 sunCol(1.0f,1.f,1.f,0);
		D3DXVECTOR3 transl = -(*m_cam->getTranslation());
		D3DXVECTOR4 camPos(transl.x, transl.y, transl.z, 1);
		D3DXVECTOR4 clip(0.f,0.f,0.f,0.f);

		ID3DX11Effect* effect =  mat->getEffect();
		ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("sceneInfo");
		if(buffer->IsValid())
		{
			
			ID3DX11EffectMatrixVariable* mat =  buffer->GetMemberByName("viewProj")->AsMatrix();
			mat->SetMatrix((float*)&viewProj);
			buffer->GetMemberByName("sunDirection")->AsVector()->SetFloatVector((float*)&sunDir);
			buffer->GetMemberByName("sunColor")->AsVector()->SetFloatVector((float*)&sunCol);
			buffer->GetMemberByName("camPos")->AsVector()->SetFloatVector((float*)&camPos);
			buffer->GetMemberByName("clipPlane")->AsVector()->SetFloatVector((float*)&clip);
			buffer->GetMemberByName("time")->AsScalar()->SetFloat(0.f);
    
    
		}
		m_previousMaterial = mat;
	}
}