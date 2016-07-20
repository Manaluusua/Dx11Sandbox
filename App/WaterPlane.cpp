#include "WaterPlane.h"
#include "CommonTypes.h"
#include "MeshUtility.h"
#include "MaterialManager.h"
#include "Material.h"
#include "Shader.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "CullableGeometry.h"
#include "Frustum.h"
#include "ReleasePtr.h"
#include "RenderContext.h"
#include "MathUtil.h"
#include "EnvironmentInfo.h"
#include "d3dx11effect.h"
WaterPlane::WaveDefinition::WaveDefinition()
    :direction(0,0),
    amplitude(0),
    frequency(0),
    phaseConstant(0)
{

}

WaterPlane::WaveDefinition::WaveDefinition(Dx11Sandbox::Vec2 dir, float amplitude, float waveLength, float speed)
    :direction(dir),
    amplitude(amplitude),
    frequency(1*2/waveLength),
    phaseConstant(speed*frequency)
{

	gmtl::normalize(direction);
}


WaterPlane::WaterPlane(Dx11Sandbox::SceneManager* mngr, ID3D11Device *device, const Dx11Sandbox::string& name, Dx11Sandbox::Vec3 normal, float d, float extends1, float extends2, float tesselationX, float tesselationY, int textureResolution)
{

	const float clipPlaneOffset = 1.f;

    m_mngr = mngr;

    m_renderObject = Dx11Sandbox::MeshUtility::createFinitePlane(device,mngr,name,normal,d,extends1, extends2, static_cast<int>( tesselationX ), static_cast<int>( tesselationY ) );
	Dx11Sandbox::RCObjectPtr<Dx11Sandbox::Material> mat = Dx11Sandbox::MaterialManager::singleton()->getOrCreateMaterial(device, "waterplane.fx",  "waterplane",m_renderObject->getRenderData().getMesh()->getInputLayout());
	m_renderObject->getRenderData().setMaterial(mat);
	m_renderObject->setRenderQueue(Dx11Sandbox::RENDERQUEUE_OPAQUE_SCENE_INPUT);
	m_renderObject->setRenderMask(Dx11Sandbox::RENDERLAYER_OPAQUE_SCENE_INPUT);

	

	initializeRenderTargets(device, mat, name,  textureResolution);

	Dx11Sandbox::string normalTexName = "waterplanenormalmap";
	m_normalMap = Dx11Sandbox::TextureManager::singleton()->getOrCreateTextureFromFile(device, "water.jpg", normalTexName, 0, D3D11_USAGE_DEFAULT, /*D3DX11_FILTER_NONE*/0);
	mat->setTexture("normalmap", m_normalMap->getName());
    
    m_normal = normal;
    m_d = d;


    //some wave values
	m_waves[0] = WaveDefinition(Dx11Sandbox::Vec2(1.f, 0.8f), 1.f, 50.f, 15.f);
	m_waves[1] = WaveDefinition(Dx11Sandbox::Vec2(0.f, 1.f), 1.4f, 20.f, 8.f);
	m_waves[2] = WaveDefinition(Dx11Sandbox::Vec2(0.7f, 0.7f), 0.7f, 30.f, 18.f);
    
    setupWaves();
	
	m_reflectionCamera = m_mngr->createCamera();
	m_reflectionCamera->setReflectionPlane(m_normal,m_d);
	m_reflectionCamera->setReflectionEnabled(true);
	Dx11Sandbox::Vec4 clipplane(m_normal[0], m_normal[1], m_normal[2], m_d + clipPlaneOffset);
	m_reflectionCamera->setClipPlane(clipplane);
	m_reflectionCamera->setRenderMask(Dx11Sandbox::RENDERLAYER_DEFAULT_OPAQUE | Dx11Sandbox::RENDERLAYER_SKYBOX | Dx11Sandbox::RENDERLAYER_LIGHTPASS);
	m_reflectionCamera->addRenderListener(this);
	m_reflectionCamera->setCameraPriority(-1);
	
	
	m_refractionCamera = m_mngr->createCamera();
	clipplane = Dx11Sandbox::Vec4(-m_normal[0], -m_normal[1], -m_normal[2], -m_d + clipPlaneOffset);
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
	int texHeight = static_cast<int>(( 1.f / Dx11Sandbox::EnvironmentInfo::getScreenAspectRatio() ) * texWidth);

	waterplaneViewport.MaxDepth = 1.f;
	waterplaneViewport.MinDepth = 0.f;
	waterplaneViewport.Height = static_cast<FLOAT>(texHeight);
	waterplaneViewport.Width = static_cast<FLOAT>(texWidth);
	waterplaneViewport.TopLeftY = 0;
	waterplaneViewport.TopLeftX = 0;


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

	m_reflection = Dx11Sandbox::TextureManager::singleton()->createTexture(reflTexName);
	m_reflection->createResource(device, texWidth, texHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
    mat->setTexture("reflection",reflTexId);


	m_refraction = Dx11Sandbox::TextureManager::singleton()->createTexture(refrTexName);
	m_refraction->createResource(device, texWidth, texHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	mat->setTexture("refraction",refrTexId);

	
	m_depthStencil = Dx11Sandbox::TextureManager::singleton()->createTexture(dsTexName);
	m_depthStencil->createResource(device, texWidth, texHeight, true, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D32_FLOAT);

}

void WaterPlane::setupWaves()
{
	ID3DX11Effect* effect =  m_renderObject->getRenderData().getMaterial()->getShader()->getEffect();
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waveDefinitions");
	Dx11Sandbox::Matrix matrix;
    matrix[0][0] = m_waves[0].direction[0];
    matrix[0][1] = m_waves[0].direction[1];
	matrix[0][2] = m_waves[0].frequency;
	matrix[0][3] = m_waves[0].amplitude;

    matrix[1][0] = m_waves[1].direction[0];
    matrix[1][1] = m_waves[1].direction[1];
    matrix[1][2] = m_waves[1].frequency;
    matrix[1][3] = m_waves[1].amplitude;

    matrix[2][0] = m_waves[2].direction[0];
    matrix[2][1] = m_waves[2].direction[1];
    matrix[2][2] = m_waves[2].frequency;
    matrix[2][3] = m_waves[2].amplitude;

    matrix[3][0] = m_waves[0].phaseConstant;
    matrix[3][1] = m_waves[1].phaseConstant;
    matrix[3][2] = m_waves[2].phaseConstant;
    matrix[3][3] = 0;

    if(buffer->IsValid())
    {
        buffer->GetMemberByName("waves")->AsMatrix()->SetMatrix((float*)&matrix);
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


Dx11Sandbox::Texture* WaterPlane::getReflectionTexture()
{
	return m_reflection;
}

Dx11Sandbox::Texture* WaterPlane::getRefractionTexture()
{
	return m_refraction;
}

void WaterPlane::setupReflectionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	state->clearState();

    //unbind reflection and refraction textures
	ID3DX11Effect* effect =  m_renderObject->getRenderData().getMaterial()->getShader()->getEffect();

    //make sure the rendertargets are not bound to pipeline
    effect->GetVariableByName("reflection")->AsShaderResource()->SetResource(0);
    effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, state->getImmediateContext());

    


    //reflection
    ID3D11RenderTargetView * views[1];
    views[0] = m_reflection->getRenderTargetView();
	state->pushRenderTargets(1, views, m_depthStencil->getDepthStencilView());
    
	state->getImmediateContext()->ClearDepthStencilView(m_depthStencil->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
    
    ID3D11RasterizerState *originalRS, *newRS;
    
	state->pushViewports(1, &waterplaneViewport);

    D3D11_RASTERIZER_DESC rdesc;
    ID3D11DeviceContext* ic = state->getImmediateContext();
    ic->RSGetState(&originalRS);
    originalRS->GetDesc(&rdesc);
    rdesc.CullMode = D3D11_CULL_FRONT;
    state->getDevice()->CreateRasterizerState(&rdesc,&newRS);

    Dx11Sandbox::ReleasePtr<ID3D11RasterizerState> original( originalRS, false), cw( newRS, false );
    ic->RSSetState(cw);

	const Dx11Sandbox::Matrix *viewRefl = camera.getViewMatrix();

	const Dx11Sandbox::Matrix *proj = camera.getProjectionMatrix();

	Dx11Sandbox::Matrix viewProjRefl = (*viewRefl) * (*proj);

    //set proj matrices to waterplane
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waterPlaneInfo");
    if(buffer->IsValid())
    {
        buffer->GetMemberByName("reflectionViewProj")->AsMatrix()->SetMatrix((float*)&viewProjRefl);
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
	state->popViewports();
}


void WaterPlane::setupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	state->clearState();
	Dx11Sandbox::RenderCamera* cam = m_mngr->getMainCamera();

	ID3DX11Effect* effect =  m_renderObject->getRenderData().getMaterial()->getShader()->getEffect();

    effect->GetVariableByName("refraction")->AsShaderResource()->SetResource(0);
    effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, state->getImmediateContext());

    

	state->pushViewports(1, &waterplaneViewport);
    //reflection
    ID3D11RenderTargetView * views[1];
    views[0] = m_refraction->getRenderTargetView();
	state->pushRenderTargets(1, views, m_depthStencil->getDepthStencilView());
    state->getImmediateContext()->ClearDepthStencilView(m_depthStencil->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
  
    const Dx11Sandbox::Matrix *viewRefr = camera.getViewMatrix();
	const Dx11Sandbox::Matrix* proj = camera.getProjectionMatrix();
	Dx11Sandbox::Matrix viewProjRefr = (*viewRefr) * (*proj);

    //set proj matrices to waterplane
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waterPlaneInfo");
    if(buffer->IsValid())
    {
        buffer->GetMemberByName("refractionViewProj")->AsMatrix()->SetMatrix((float*)&viewProjRefr);
    }

}

void WaterPlane::cleanupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state)
{
	//cleanup
	state->popRenderTargets();
	state->popViewports();
}
