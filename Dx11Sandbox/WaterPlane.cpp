#include "WaterPlane.h"
#include "MeshUtility.h"
#include "MaterialManager.h"
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "RenderObject.h"
#include "Frustrum.h"

#include "MathUtil.h"

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


WaterPlane::WaterPlane(Dx11Sandbox::SceneManager* mngr,ID3D11Device *device, const Dx11Sandbox::string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2, float tesselationX, float tesselationY)
{

    mngr->addRenderStartListener(this);

    m_renderObject = Dx11Sandbox::MeshUtility::createFinitePlane(device,mngr,name,normal,d,extends1, extends2,tesselationX,tesselationY);
    (*m_renderObject)->mat = Dx11Sandbox::MaterialManager::getSingleton()->getOrCreateMaterial(device, L"waterplane.fx",  L"waterplane",Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);
    (*m_renderObject)->renderQueueFlag = Dx11Sandbox::RSCENEINPUT;

    //hack, always use same name. Later on take the name from name passed in to avoid name collision when multiple waterplanes in the scene
    Dx11Sandbox::wstring reflTexName = L"waterplanereflection";
    m_reflection = Dx11Sandbox::TextureManager::getSingleton()->getOrCreateTexture(device,reflTexName,mngr->getScreenWidth(),mngr->getScreenHeight(),1,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_USAGE_DEFAULT);
    (*m_renderObject)->mat->setTexture("reflection",reflTexName);

    Dx11Sandbox::wstring refrTexName = L"waterplanerefraction";
    m_refraction = Dx11Sandbox::TextureManager::getSingleton()->getOrCreateTexture(device,refrTexName,mngr->getScreenWidth(),mngr->getScreenHeight(),1,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_USAGE_DEFAULT);
    (*m_renderObject)->mat->setTexture("refraction",refrTexName);

    m_normal = normal;
    m_d = d;


    //some wave values
    m_waves[0] = WaveDefinition(D3DXVECTOR2(1,0),3,120,15);
    m_waves[1] = WaveDefinition(D3DXVECTOR2(0,1),2.f,20,8);
    m_waves[2] = WaveDefinition(D3DXVECTOR2(0.7f,0.7f),2,30,18);
    
    setupWaves();
}


WaterPlane::~WaterPlane(void)
{

}


void WaterPlane::setupWaves()
{
    ID3DX11Effect* effect =  (*m_renderObject)->mat->getEffect();
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

void WaterPlane::renderingStarted(Dx11Sandbox::RenderContext* context,Dx11Sandbox::SceneManager* mngr, double fTime, float fElapsedTime)
{

    mngr->getMainCamera().setReflectionPlane(m_normal,m_d);
    context->clearState();
    //unbind reflection and refraction textures
    ID3DX11Effect* effect =  (*m_renderObject)->mat->getEffect();

    //make sure the rendertargets are not bound to pipeline
    effect->GetVariableByName("refraction")->AsShaderResource()->SetResource(0);
    effect->GetVariableByName("reflection")->AsShaderResource()->SetResource(0);
    effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, context->getImmediateContext());

    D3DXVECTOR4 clipplane(m_normal, m_d);

    Dx11Sandbox::Camera* cam = &mngr->getMainCamera();

    //reflection
    ID3D11RenderTargetView * views[1];
    views[0] = m_reflection->GetRenderTargetView();
    context->getImmediateContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );
    context->bindRenderTargets(1, views, DXUTGetD3D11DepthStencilView());
    
    ID3D11RasterizerState *original, *cw;
    D3D11_RASTERIZER_DESC rdesc;
    ID3D11DeviceContext* ic = context->getImmediateContext();
    ic->RSGetState(&original);
    original->GetDesc(&rdesc);
    rdesc.CullMode = D3D11_CULL_FRONT;
    context->getDevice()->CreateRasterizerState(&rdesc,&cw);
    ic->RSSetState(cw);
    
    cam->setReflectionEnabled(true);

    context->setCustomClipPlane(clipplane);



    Dx11Sandbox::Frustrum frust;
    cam->calculateFrustrum(&frust);
    mngr->cullObjectsToRenderQueues(frust);

    mngr->renderQueue(fTime,fElapsedTime,cam,Dx11Sandbox::RDEFAULT, mngr->getDefaultRenderer());
    mngr->renderQueue(fTime,fElapsedTime,cam,Dx11Sandbox::RFINAL, mngr->getDefaultRenderer());

    context->clearState();
    // set changed waterplane uniforms

    const D3DXMATRIX *view = cam->getViewMatrix();
    const D3DXMATRIX *proj = cam->getProjectionMatrix();
    D3DXMATRIX viewProj =  (*view) * (*proj);

    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("waterPlaneInfo");
    if(buffer->IsValid())
    {
        buffer->GetMemberByName("reflectionViewProj")->AsMatrix()->SetMatrix((float*)viewProj);
    }


    cam->setReflectionEnabled(false);

    ic->RSSetState(original);
    
    //refraction
    clipplane = D3DXVECTOR4(-m_normal, -m_d);
    context->setCustomClipPlane(clipplane);
   
    views[0] = m_refraction->GetRenderTargetView();
    context->getImmediateContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );
    context->bindRenderTargets(1, views, DXUTGetD3D11DepthStencilView());

    cam->calculateFrustrum(&frust);
    mngr->cullObjectsToRenderQueues(frust);

    mngr->renderQueue(fTime,fElapsedTime,&mngr->getMainCamera(),Dx11Sandbox::RDEFAULT, mngr->getDefaultRenderer());
    
    
    context->clearState();
    //cleanup
    context->bindBackBuffer();

    cw->Release();
    original->Release();

   
    clipplane = D3DXVECTOR4(0,0,0,0);
    context->setCustomClipPlane(clipplane);
}