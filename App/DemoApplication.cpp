#include "DemoApplication.h"
#include "SceneManager.h"
#include "MeshUtility.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "Shader.h"
#include "RenderCamera.h"
#include "MeshManager.h"
#include "RenderContext.h"
#include "TerrainBinHandler.h"
#include "CullableGeometry.h"
#include "CullableLight.h"
#include "DebugDrawTextureToScreen.h"
#include "Texture.h"
#include "d3dx11effect.h"

DemoApplication::DemoApplication()
    :m_leftDown(false),
    m_rightDown(false),
    m_forwardDown(false),
    m_backwardDown(false),
    m_upDown(false),
    m_downDown(false),
    m_moveMouse(false),
    m_lastMousePos(-1,-1),
    m_mouseDelta(0,0),
	m_mouseSensitivity(0.01f),
    m_time(0),
    m_waterPlane(0),
	m_debugDrawerTexture(0)
{

}


void DemoApplication::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown)
{
    switch(nChar)
    {
    case 'W':
        m_forwardDown = bKeyDown;
        break;
    case 'S':
        m_backwardDown = bKeyDown;
        break;
    case 'A':
        m_leftDown = bKeyDown;
        break;
    case 'D':
        m_rightDown = bKeyDown;
        break;
    case 'Q':
        m_upDown = bKeyDown;
        break;
    case 'E':
        m_downDown = bKeyDown;
        break;
    }
    
}

void DemoApplication::handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
        
}

void DemoApplication::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                    bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                    int xPos, int yPos )
{


    if(bLeftButtonDown)
    {
        m_moveMouse = true;
    }else
    {
        m_moveMouse = false;
    }

    
}


void DemoApplication::windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{

}



void DemoApplication::createWorld(SceneManager* mngr)
{
    
    m_mngr = mngr;
    Camera* cam = mngr->getMainCamera();

    ID3D11Device* device = mngr->getRenderContext().getDevice();
    //camera
    D3DXVECTOR3 vecEye( 0.0f, 90.0f, 60.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 88.0f, 70.0f );
    D3DXVECTOR3 up (0.0f,1.0f,0.0f);
    cam->lookAt(vecEye, vecAt, up);

        
    //objects
    Material* mat = 0; 
    Mesh* mesh = 0; 
    CullableGeometry *ro;

    //skybox
    mat = MaterialManager::singleton()->getOrCreateMaterial(device, "skybox.fx", "skybox",MeshInputLayouts::POS3TEX3);
    mesh = MeshUtility::createSkyBoxMesh(device, "skybox" + generateID());
	ro = mngr->createCullableGeometry();
	ro->setBoundingSphere( D3DXVECTOR4( 0,0,0, FLT_MAX ) );
	ro->setMaterial( mat );
	ro->setMesh( mesh );
	ro->setRenderQueue(RENDERQUEUE_SKYBOX);
	ro->setRenderMask(RENDERLAYER_SKYBOX);

	Texture* tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, "skyboxCube.dds", "skyboxCube.dds");

    mat->setTexture("cubemap", tex->getName());
    
     
    //terrain
    mat = MaterialManager::singleton()->getOrCreateMaterial(device, "terrain.fx", "terrain1",MeshInputLayouts::POS3NORM3TEX2);
    MeshUtility::createTerrainFromHeightMap(device,mngr, "heightmapTerrain.png", mat,1000,1000,200,20,20,10);

    //textures
    //mat->setTexture("texture1", L"roughRock.png");
    //TextureManager::getSingleton()->createTexture(device, L"roughRock.png", L"roughRock.png");

	tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, "grass.jpg", "grass.jpg");
    mat->setTexture("texture1", tex->getName());
    

 
    //mat->setTexture("textureWeights", L"terrainweights.png");
    //TextureManager::getSingleton()->createTexture(device, L"terrainweights.png", L"terrainweights.png");
    


    //waterplane
    Dx11Sandbox::string name("waterPlane1");
	m_waterPlane = new WaterPlane(mngr,device, name,D3DXVECTOR3(0,1,0),-60,340,340,200,200, 512);
    
	m_debugDrawerTexture = new DebugDrawTextureToScreen(device, 100.f, 100.f);
	m_mngr->addDebugDrawer(m_debugDrawerTexture);
	m_debugDrawerTexture->addDebugTexture(m_waterPlane->getRefractionTexture(), 40.f, 40.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture(m_waterPlane->getReflectionTexture(), 40.f, 20.f, 20.f, 20.f);

	m_debugDrawerTexture->addDebugTexture("DepthStencil", -40.f, -20.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_ALBEDO",-40.f, 40.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_NORMAL", -40.f, 20.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_SPECULAR", -40.f, 0.f, 20.f, 20.f);
	

	sun = m_mngr->createLight();
	sun->setLightType(Dx11Sandbox::Light::DIRECTIONAL);
	sun->setColor(D3DXVECTOR4(1.0f,1.f,1.f,0));

}
void DemoApplication::update(SceneManager* mngr,double fTime, float fElapsedTime)
{
    m_time += fElapsedTime;
    handleInput(mngr,fElapsedTime, static_cast<float>( fTime ) );

	D3DXVECTOR4 sunDir( std::cos( m_time ), 0.f, std::sin( m_time ),0 );
    D3DXVec4Normalize(&sunDir, &sunDir);
	sun->setLightParameters(sunDir);
}


void DemoApplication::handleInput(SceneManager* mngr, float dt, float elapsedTime)
{
    Camera* cam = mngr->getMainCamera();

    //cam movement
    //keyboard
    D3DXVECTOR3 mov(0,0,0);
    float speed = 40;
    float mouseSens = 1;

    if(m_leftDown)
    {
        mov += D3DXVECTOR3(-1,0,0)*speed*dt;
    }
    if(m_rightDown)
    {
        mov += D3DXVECTOR3(1,0,0)*speed*dt;
    }
    if(m_forwardDown)
    {
        mov += D3DXVECTOR3(0,0,1)*speed*dt;
    }
    if(m_backwardDown)
    {
        mov += D3DXVECTOR3(0,0,-1)*speed*dt;
    }
    if(m_upDown)
    {
        mov += D3DXVECTOR3(0,1,0)*speed*dt;
    }
    if(m_downDown)
    {
        mov += D3DXVECTOR3(0,-1,0)*speed*dt;
    }

    if(mov!=D3DXVECTOR3(0,0,0))
    {
        cam->moveCameraViewRelative(mov.x, mov.y, mov.z);
    }





    //le mouse
    POINT mousePos;
    D3DXVECTOR2 mousePosVec;



    if(m_moveMouse)
    {
            // Get current position of mouse
        GetCursorPos( &mousePos );
        mousePosVec = D3DXVECTOR2( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
        if(m_lastMousePos == D3DXVECTOR2(-1,-1))
        {
            m_lastMousePos = mousePosVec;
        }else
        {
            m_mouseDelta = mousePosVec - m_lastMousePos;
            m_lastMousePos = mousePosVec;
        }
    }else
    {
        m_mouseDelta = D3DXVECTOR2(0,0);
        m_lastMousePos = D3DXVECTOR2(-1,-1);
    }





    if(m_mouseDelta != D3DXVECTOR2(0,0))
    {
        cam->rotateCameraViewRelative(-m_mouseDelta.y*mouseSens*m_mouseSensitivity ,-m_mouseDelta.x*mouseSens*m_mouseSensitivity, 0);
    }

}

void DemoApplication::shutDown(SceneManager* mngr)
{
	SAFE_DELETE(m_debugDrawerTexture);
	m_waterPlane = 0;
}


void DemoApplication::objectBeingRendered(CullableGeometry* obj)
{
	//Tämä pois kokonaan lopulta, engine hoitaa. For now laita listeneri rendereriin (saa staten ja listan renderdatoista)
	Dx11Sandbox::RenderContext& state = m_mngr->getRenderContext();

	Material* mat = obj->getMaterial();

    //new pass, don't try to skip effect state setting
    


        //D3DXMATRIX *world;
	const D3DXMATRIX *view = m_mngr->getMainCamera()->getViewMatrix();
    const D3DXMATRIX *proj = m_mngr->getMainCamera()->getProjectionMatrix();
    D3DXMATRIX viewProj =  (*view) * (*proj);

        //temp
    D3DXVECTOR4 sunDir( std::cos( m_time ), 0.f, std::sin( m_time ),0 );
    D3DXVec4Normalize(&sunDir, &sunDir);
    D3DXVECTOR4 sunCol(1.0f,1.f,1.f,0);
    D3DXVECTOR3 transl = -(m_mngr->getMainCamera()->getTranslation());
    D3DXVECTOR4 camPos(transl.x, transl.y, transl.z, 1);

    ID3DX11Effect* effect =  mat->getShader()->getEffect();
    ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("sceneInfo");
    if(buffer->IsValid())
    {
			
		ID3DX11EffectMatrixVariable* mat =  buffer->GetMemberByName("viewProj")->AsMatrix();
        mat->SetMatrix((float*)&viewProj);
        buffer->GetMemberByName("sunDirection")->AsVector()->SetFloatVector((float*)&sunDir);
        buffer->GetMemberByName("sunColor")->AsVector()->SetFloatVector((float*)&sunCol);
        buffer->GetMemberByName("camPos")->AsVector()->SetFloatVector((float*)&camPos);
        buffer->GetMemberByName("clipPlane")->AsVector()->SetFloatVector((float*)&state.getCustomClipPlane());
        buffer->GetMemberByName("time")->AsScalar()->SetFloat(m_time);
    
    }
}