#include "WaterPlane.h"
#include "CommonTypes.h"
#include "MeshUtility.h"
#include "MaterialManager.h"
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "CullableGeometry.h"
#include "Frustrum.h"
#include "ReleasePtr.h"
#include "MathUtil.h"
#include "EnvironmentInfo.h"

WaterPlane::WaveDefinition::WaveDefinition()
    :direction(0,0),
    amplitude(0),
    frequency(0),
    phaseConstant(0)
{

}

WaterPlane::WaveDefinition::WaveDefinition(D3DXVECTOR2 dir,float amplitude, float waveLength, float speed)
    :direction(dir),
    amplitude(amplitude),
    frequency(1*2/waveLength),
    phaseConstant(speed*frequency)
{
    D3DXVec2Normalize(&direction,&direction);
}


WaterPlane::WaterPlane(Dx11Sandbox::SceneManager* mngr,ID3D11Device *device, const Dx11Sandbox::string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2, float tesselationX, float tesselationY, int textureResolution)
{

    m_mngr = mngr;

    m_renderObject = Dx11Sandbox::MeshUtility::createFinitePlane(device,mngr,name,normal,d,extends1, extends2, static_cast<int>( tesselationX ), static_cast<int>( tesselationY ) );
	Dx11Sandbox::RCObjectPtr<Dx11Sandbox::Material> mat = Dx11Sandbox::MaterialManager::singleton()->getOrCreateMaterial(device, "waterplane.fx",  "waterplane",Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);
	m_renderObject->setMaterial(mat);
	m_renderObject->setRenderQueue(Dx11Sandbox::RENDERQUEUE_OPAQUE_SCENE_INPUT);
	m_renderObject->setRenderMask(Dx11Sandbox::RENDERLAYER_OPAQUE_SCENE_INPUT);

	

	initializeRenderTargets(device, mat, name,  textureResolution);

	Dx11Sandbox::ResourceID normalTexName = Dx11Sandbox::stringToID("waterplanenormalmap");
    m_normalMap = Dx11Sandbox::TextureManager::singleton()->getOrCreateTexture(device, "waternormal.png", normalTexName, 0, D3D11_USAGE_DEFAULT, D3DX11_FILTER_POINT );
    mat->setTexture("normalmap",normalTexName);
    
    m_normal = normal;
    m_d = d;


    //some wave values
    m_waves[0] = WaveDefinition(D3DXVECTOR2(1.f,0.8f),1.f,50.f,15.f);
    m_waves[1] = WaveDefinition(D3DXVECTOR2(0.f,1.f),1.4f,20.f,8.f);
    m_waves[2] = WaveDefinition(D3DXVECTOR2(0.7f,0.7f),0.7f,30.f,18.f);
    
    setupWaves();
	
	m_reflectionCamera = m_mngr->createCamera();
	m_reflectionCamera->setReflectionPlane(m_normal,m_d);
	m_reflectionCamera->setReflectionEnabled(true);
	D3DXVECTOR4 clipplane(m_normal, m_d);
	m_reflectionCamera->setClipPlane(clipplane);
	m_reflectionCamera->setRenderMask(Dx11Sandbox::RENDERLAYER_DEFAULT_OPAQUE | Dx11Sandbox::RENDERLAYER_SKYBOX | Dx11Sandbox::RENDERLAYER_LIGHTPASS);
	m_reflectionCamera->addRenderListener(this);
	m_reflectionCamera->setCameraPriority(-1);
	
	
	m_refractionCamera = m_mngr->createCamera();
	clipplane = D3DXVECTOR4(-m_normal, -m_d);
	m_refractionCamera->setClipPlane(clipplane);
	m_refractionCamera->setRenderMask(Dx11Sandbox::RENDERLAYER_DEFAULT_OPAQUE | Dx11Sandbox::RENDERLAYER_SKYBOX | Dx11Sandbox::RENDERLAYER_LIGHTPASS);
	m_refractionCamera->addRenderListener(this);
	m_refractionCamera->setCameraPriority(-1);

}


WaterPlane::~WaterPlane(void)
{
	m_mngr->destroyCamera(m_reflectionCamera);
	m_mngr->destroyCamera(m_refractionCamera);
}

void WaterPlane::initializeRenderTargets(ID3D11Device *device, Dx11Sandbox::RCObjectPtr<Dx11Sandbox::Material> mat, const Dx11Sandbox::string& name, int resolution)
{

	int texWidth = resolution;
	int texHeight = ( 1.f / Dx11Sandbox::EnvironmentInfo::getScreenAspectRatio() ) * texWidth;


	Dx11Sandbox::string reflTexName = "waterReflection";
	reflTexName += name;
	Dx11Sandbox::string refrTexName = "waterRefraction";
	refrTexName += name;
	Dx11Sandbox::string dsTexName = "waterplanedepthstencil";
	dsTexName += name;

	Dx11Sandbox::ResourceID reflTexId = Dx11Sandbox::stringToID(reflTexName);
	Dx11Sandbox::ResourceID refrTexId = Dx11Sandbox::stringToID(refrTexName);

	Dx11Sandbox::TextureManager::singleton()->releaseTexture(reflTexId);
	Dx11Sandbox::TextureManager::singleton()->releaseTexture(refrTexId);
	Dx11Sandbox::TextureManager::singleton()->releaseTexture(Dx11Sandbox::stringToID(dsTexName));

	m_reflection = Dx11Sandbox::TextureManager::singleton()->createTexture2D(device,reflTexName, texWidth, texHeight,1,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,DXGI_FORMAT_R32G32B32A32_FLOAT ,0,D3D11_USAGE_DEFAULT);
    mat->setTexture("reflection",reflTexId);

    
	m_refraction = Dx11Sandbox::TextureManager::singleton()->createTexture2D(device,refrTexName, texWidth, texHeight ,1,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE ,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_USAGE_DEFAULT);
	mat->setTexture("refraction",refrTexId);

	
	m_depthStencil = Dx11Sandbox::TextureManager::singleton()->createTexture2D(device,dsTexName, texWidth, texHeight ,1,D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D32_FLOAT ,0,D3D11_USAGE_DEFAULT);

}

void WaterPlane::setupWaves()
{
	ID3DX11Effect* effect =  m_renderObject->getMaterial()->getEffect();
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waveDefinitions");
    D3DXMATRIX matrix;
    matrix._11 = m_waves[0].direction.x;
    matrix._12 = m_waves[0].direction.y;
    matrix._13 = m_waves[0].frequency;
    matrix._14 = m_waves[0].amplitude;

    matrix._21 = m_waves[1].direction.x;
    matrix._22 = m_waves[1].direction.y;
    matrix._23 = m_waves[1].frequency;
    matrix._24 = m_waves[1].amplitude;

    matrix._31 = m_waves[2].direction.x;
    matrix._32 = m_waves[2].direction.y;
    matrix._33 = m_waves[2].frequency;
    matrix._34 = m_waves[2].amplitude;

    matrix._41 = m_waves[0].phaseConstant;
    matrix._42 = m_waves[1].phaseConstant;
    matrix._43 = m_waves[2].phaseConstant;
    matrix._44 = 0;

    if(buffer->IsValid())
    {
        buffer->GetMemberByName("waves")->AsMatrix()->SetMatrix((float*)matrix);
    }
}

void WaterPlane::cameraPreCull(Dx11Sandbox::RenderCamera& camera)
{
	Dx11Sandbox::RenderCamera* cam = m_mngr->getMainCamera();
	
	camera.copyCameraViewAndProjectionParameters(*cam);
	camera.setFOVY(camera.getFOVY() * 1.2f);
}

void WaterPlane::cameraStartedRendering(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderBin& renderbin, Dx11Sandbox::RenderContext* state)
{
	if(&camera == m_reflectionCamera)
	{
		setupReflectionCamera(camera, state);
	}else if(&camera == m_refractionCamera)
	{
		setupRefractionCamera(camera, state);
	}
}

void WaterPlane::cameraEndedRendering(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderBin& renderbin, Dx11Sandbox::RenderContext* state)
{
	if(&camera == m_reflectionCamera)
	{
		cleanupReflectionCamera(camera, state);
	}else if(&camera == m_refractionCamera)
	{
		cleanupRefractionCamera(camera, state);
	}
}

void WaterPlane::update(double fTime, float fElapsedTime)
{

}

void WaterPlane::setupReflectionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	state->clearState();

    //unbind reflection and refraction textures
	ID3DX11Effect* effect =  m_renderObject->getMaterial()->getEffect();

    //make sure the rendertargets are not bound to pipeline
    effect->GetVariableByName("reflection")->AsShaderResource()->SetResource(0);
    effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, state->getImmediateContext());

    


    //reflection
    ID3D11RenderTargetView * views[1];
    views[0] = m_reflection->GetRenderTargetView();
	state->pushRenderTargets(1, views, m_depthStencil->GetDepthStencilView());
    
	state->getImmediateContext()->ClearDepthStencilView(m_depthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
    
    ID3D11RasterizerState *originalRS, *newRS;
    

    D3D11_RASTERIZER_DESC rdesc;
    ID3D11DeviceContext* ic = state->getImmediateContext();
    ic->RSGetState(&originalRS);
    originalRS->GetDesc(&rdesc);
    rdesc.CullMode = D3D11_CULL_FRONT;
    state->getDevice()->CreateRasterizerState(&rdesc,&newRS);

    Dx11Sandbox::ReleasePtr<ID3D11RasterizerState> original( originalRS, false), cw( newRS, false );
    ic->RSSetState(cw);

	const D3DXMATRIX *viewRefl = camera.getViewMatrix();

    const D3DXMATRIX *proj = camera.getProjectionMatrix();

    D3DXMATRIX viewProjRefl =  (*viewRefl) * (*proj);

    //set proj matrices to waterplane
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waterPlaneInfo");
    if(buffer->IsValid())
    {
        buffer->GetMemberByName("reflectionViewProj")->AsMatrix()->SetMatrix((float*)viewProjRefl);
    }


}

void WaterPlane::cleanupReflectionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	//cleanup
	state->popRenderTargets();
	
	ID3D11RasterizerState *originalRS, *newRS;
    
    D3D11_RASTERIZER_DESC rdesc;
    ID3D11DeviceContext* ic = state->getImmediateContext();
    ic->RSGetState(&originalRS);
    originalRS->GetDesc(&rdesc);
	rdesc.CullMode = D3D11_CULL_BACK;
    state->getDevice()->CreateRasterizerState(&rdesc,&newRS);

    Dx11Sandbox::ReleasePtr<ID3D11RasterizerState> original( originalRS, false), ccw( newRS, false );
    ic->RSSetState(ccw);
}


void WaterPlane::setupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	state->clearState();
	Dx11Sandbox::RenderCamera* cam = m_mngr->getMainCamera();

	ID3DX11Effect* effect =  m_renderObject->getMaterial()->getEffect();

    effect->GetVariableByName("refraction")->AsShaderResource()->SetResource(0);
    effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, state->getImmediateContext());

    


    //reflection
    ID3D11RenderTargetView * views[1];
    views[0] = m_refraction->GetRenderTargetView();
	state->pushRenderTargets(1, views, m_depthStencil->GetDepthStencilView());
    state->getImmediateContext()->ClearDepthStencilView(m_depthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
  
    const D3DXMATRIX *viewRefr = camera.getViewMatrix();
    const D3DXMATRIX *proj = camera.getProjectionMatrix();
    D3DXMATRIX viewProjRefr =  (*viewRefr) * (*proj);

    //set proj matrices to waterplane
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waterPlaneInfo");
    if(buffer->IsValid())
    {
        buffer->GetMemberByName("refractionViewProj")->AsMatrix()->SetMatrix((float*)viewProjRefr);
    }

}

void WaterPlane::cleanupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	//cleanup
	state->popRenderTargets();

}
