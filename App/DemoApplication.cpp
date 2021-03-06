#include "DemoApplication.h"
#include "SceneManager.h"
#include "MeshUtility.h"
#include "TextureManager.h"
#include "MathUtil.h"
#include "MaterialManager.h"
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderCamera.h"
#include "MeshManager.h"
#include "RenderContext.h"
#include "TerrainBinHandler.h"
#include "RenderData.h"
#include "CullableGeometry.h"
#include "CullableLight.h"
#include "DebugDrawTextureToScreen.h"
#include "DebugDrawLights.h"
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
	m_debugDrawerTexture(0),
	m_debugDrawerLights(0)
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
	case 'L':
		if (bKeyDown){
			m_debugDrawerLights->setEnabled(!m_debugDrawerLights->isEnabled());
		}
		break;

	case 'T':
		if (bKeyDown){
			m_debugDrawerTexture->setEnabled(!m_debugDrawerTexture->isEnabled());
		}
		
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
    
    mesh = MeshUtility::createSkyBoxMesh(device, "skybox" + generateID());
	mat = MaterialManager::singleton()->getOrCreateMaterial(device, "skybox.fx", "skybox", mesh->getInputLayout());
	ro = mngr->createCullableGeometry();
	ro->setBoundingSphere( D3DXVECTOR4( 0,0,0, FLT_MAX ) );
	ro->getRenderData().setMaterial( mat );
	ro->getRenderData().setMesh( mesh );
	ro->setRenderQueue(RENDERQUEUE_SKYBOX);
	ro->setRenderMask(RENDERLAYER_SKYBOX);
	Texture* tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, "skyboxCube.dds", "skybox");
	mat->setTexture("cubemap", tex->getName());
    
     
    //terrain
	InputLayoutDescription inputDescription;
	inputDescription.appendDescription("POSITION", DXGI_FORMAT_R32G32B32_FLOAT).appendDescription("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT).appendDescription("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT).appendDescription("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	mat = MaterialManager::singleton()->getOrCreateMaterial(device, "terrain.fx", "terrain1", inputDescription);
    MeshUtility::createTerrainFromHeightMap(device,mngr, "heightmapTerrain.png", mat,1000,1000,200,30,30,10);
	 tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, "grass.jpg", "grass.jpg");
    mat->setTexture("texture1", tex->getName());

    //waterplane
    Dx11Sandbox::string name("waterPlane1");
	m_waterPlane = new WaterPlane(mngr,device, name,D3DXVECTOR3(0,1,0),-40,300,300,100,100, 512);
    
	//lights
	sun = m_mngr->createLight();
	sun->setLightType(Dx11Sandbox::Light::DIRECTIONAL);
	sun->setColor(D3DXVECTOR3(0.2f,0.2f,0.2f));
	D3DXVECTOR3 sunDir( 1.f, -1.f, 0.f );
	D3DXVec3Normalize(&sunDir, &sunDir);
	sun->setDirection(sunDir);



	CullableLight* l;
	unsigned int lightsGenerated = 180;
	float lightRadMin = 80.f;
	float lightRadMax = 150.f;
	float circleRadMin = 100.f;
	float circleRadMax = 800.f;
	float maxHeight = 130.f;
	float minHeight = 60.f;
	D3DXVECTOR3 color;
	D3DXVECTOR3 pos;
	for (unsigned int i = 0; i < lightsGenerated; ++i){
		float rat = (static_cast<float>(i) / lightsGenerated);

		float r = 0.1f + Dx11Sandbox::MathUtil::randomFloat() * 0.9f;
		color.x = r;
		r = 0.1f + Dx11Sandbox::MathUtil::randomFloat() * 0.9f;
		color.y = r;
		r = 0.1f + Dx11Sandbox::MathUtil::randomFloat() * 0.9f;
		color.z = r;

		float circleRad = circleRadMin + (circleRadMax - circleRadMin)*Dx11Sandbox::MathUtil::randomFloat();
		float lightRad = lightRadMin + (lightRadMax - lightRadMin)*Dx11Sandbox::MathUtil::randomFloat();

		pos.x = std::cos(2.f * Dx11Sandbox::MathUtil::PI * rat) * circleRad;
		pos.z = std::sin(2.f * Dx11Sandbox::MathUtil::PI * rat) * circleRad;
		pos.y = minHeight + (maxHeight - minHeight) * Dx11Sandbox::MathUtil::randomFloat();

		l = m_mngr->createLight();
		l->setLightType(Dx11Sandbox::Light::OMNI);
		l->setRadius(lightRad);
		l->setColor(color);
		l->setPosition(pos);
		l->setLightId(i + 1);
	}
	
	createMaterialBalls(mngr);

	//debug drawers

	m_debugDrawerLights = new DebugDrawLights(device);
	m_mngr->addDebugDrawer(m_debugDrawerLights);
	
	m_debugDrawerTexture = new DebugDrawTextureToScreen(device, 100.f, 100.f);
	m_mngr->addDebugDrawer(m_debugDrawerTexture);
	m_debugDrawerTexture->addDebugTexture(m_waterPlane->getRefractionTexture(), 40.f, 40.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture(m_waterPlane->getReflectionTexture(), 40.f, 20.f, 20.f, 20.f);

	
	m_debugDrawerTexture->addDebugTexture("GBUFFER_ALBEDO", -40.f, 40.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_NORMAL", -40.f, 20.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_SPECULAR", -40.f, 0.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("GBUFFER_ENVIRONMENT", -40.f, -20.f, 20.f, 20.f);
	m_debugDrawerTexture->addDebugTexture("DepthStencil", -40.f, -40.f, 20.f, 20.f);
	

}

void createMaterialBall(SceneManager* mngr,const string& albedoTex, const string& specularTex,const string& normalTex, float x, float y){

	ID3D11Device* device = mngr->getRenderContext().getDevice();

	Material* mat = 0;
	Mesh* mesh = 0;
	CullableGeometry *ro;
	D3DXMATRIX wmatrix;
	D3DXMatrixIdentity(&wmatrix);

	mesh = MeshUtility::createUnitSphere(device, 20,20);

	mat = new Material(*MaterialManager::singleton()->getMaterial( "albedoNormalSpecular"));
	Texture* tex = TextureManager::singleton()->getTexture(stringToID("environmentMap"));
	mat->setTexture("environmentTex", tex->getName());


	tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, albedoTex, albedoTex);
	mat->setTexture("albedoTex", tex->getName());
	tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, specularTex, specularTex);
	mat->setTexture("specularTex", tex->getName());
	tex = Dx11Sandbox::TextureManager::singleton()->getOrCreateTextureFromFile(device, normalTex, normalTex, 0, D3D11_USAGE_DEFAULT, D3DX11_FILTER_NONE);
	mat->setTexture("normalTex", tex->getName());

	wmatrix._11 = 10.f;
	wmatrix._22 = 10.f;
	wmatrix._33 = 10.f;
	wmatrix._41 = x;
	wmatrix._42 = y;
	wmatrix._43 = 0.f;
	ro = mngr->createCullableGeometry();
	ro->setBoundingSphere(D3DXVECTOR4(0, 0, 0, FLT_MAX));
	ro->setWorldMatrix(wmatrix);
	ro->getRenderData().setMaterial(mat);
	ro->getRenderData().setMesh(mesh);
	ro->setRenderQueue(RENDERQUEUE_DEFAULT_OPAQUE);
	ro->setRenderMask(RENDERLAYER_DEFAULT_OPAQUE);

}

void DemoApplication::createMaterialBalls(SceneManager* mngr)
{

	ID3D11Device* device = mngr->getRenderContext().getDevice();
	InputLayoutDescription inputDescription;
	inputDescription.appendDescription("POSITION", DXGI_FORMAT_R32G32B32_FLOAT).appendDescription("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT).appendDescription("TANGENT", DXGI_FORMAT_R32G32B32A32_FLOAT).appendDescription("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	Material* mat = MaterialManager::singleton()->getOrCreateMaterial(device, "albedoNormalSpecular.fx", "albedoNormalSpecular", inputDescription);
	Texture* tex = TextureManager::singleton()->getOrCreateTextureFromFile(device, "skyboxCube2.dds", "environmentMap");

	createMaterialBall(mngr, "gold_albedo.jpg", "gold_specular.png", "gold_normal.jpg", 0.f, 60.f);
	createMaterialBall(mngr, "red_albedo.jpg", "red_specular_fine.png", "red_normal.jpg", 30.f, 60.f);
	createMaterialBall(mngr, "red_albedo.jpg", "red_specular_semifine.png", "red_normal.jpg", 60.f, 60.f);
	createMaterialBall(mngr, "red_albedo.jpg", "red_specular_coarse.png", "red_normal.jpg", 90.f, 60.f);
	createMaterialBall(mngr, "red_albedo.jpg", "red_specular_verycoarse.png", "red_normal.jpg", 120.f, 60.f);
}



void DemoApplication::update(SceneManager* mngr,double fTime, float fElapsedTime)
{
    m_time += fElapsedTime;
    handleInput(mngr,fElapsedTime, static_cast<float>( fTime ) );

	D3DXVECTOR3 sunDir( std::cos( m_time ), -0.4f, std::sin( m_time ) );
    D3DXVec3Normalize(&sunDir, &sunDir);
	sun->setDirection(sunDir);
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






    POINT mousePos;
    D3DXVECTOR2 mousePosVec;



    if(m_moveMouse)
    {

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
	SAFE_DELETE(m_debugDrawerLights);
	m_waterPlane = 0;
}


void DemoApplication::objectBeingRendered(CullableGeometry* obj)
{
	
	Dx11Sandbox::RenderContext& state = m_mngr->getRenderContext();

	Material* mat = obj->getRenderData().getMaterial();


    


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