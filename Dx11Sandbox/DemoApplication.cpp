#include "DemoApplication.h"
#include "SceneManager.h"
#include "MeshUtility.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "MeshManager.h"


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
    m_lastPassID(0),
    m_time(0),
    m_waterPlane(0)
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
    Camera& cam = mngr->getMainCamera();

    ID3D11Device* device = mngr->getRenderContext().getDevice();
    //camera
    D3DXVECTOR3 vecEye( 0.0f, 90.0f, 60.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 88.0f, 70.0f );
    D3DXVECTOR3 up (0.0f,1.0f,0.0f);
    cam.lookAt(vecEye, vecAt, up);
        
    mngr->getRenderBinHandler().addRenderBinListener( this );

        
    //objects
    Material* mat = 0; 
    Mesh* mesh = 0; 
    CullInfo **ro;

    //skybox
    mat = MaterialManager::getSingleton()->getOrCreateMaterial(device, L"skybox.fx", L"skybox",MeshInputLayouts::POS3TEX3);
    mesh = MeshUtility::createSkyBoxMesh(device, "skybox" + generateID());
    ro = mngr->allocateDynamic();
    (*ro)->boundingSphere = D3DXVECTOR4( 0,0,0, FLT_MAX );
    (*ro)->mat = mat;
    (*ro)->mesh = mesh;
    (*ro)->binIDFlag = mngr->getRenderBinHandler().getIDForBinName( Dx11Sandbox::RENDERBIN_SKYBOX );

    mat->setTexture("cubemap", L"skyboxCube.dds");
    TextureManager::getSingleton()->createTexture(device, L"skyboxCube.dds", L"skyboxCube.dds");
     
    //terrain
    mat = MaterialManager::getSingleton()->getOrCreateMaterial(device, L"terrain.fx", L"terrain1",MeshInputLayouts::POS3NORM3TEX2);
    MeshUtility::createTerrainFromHeightMap(device,mngr, L"heightmapTerrain.png", mat,1000,1000,200,40,40,10);

    //textures
    //mat->setTexture("texture1", L"roughRock.png");
    //TextureManager::getSingleton()->createTexture(device, L"roughRock.png", L"roughRock.png");


    mat->setTexture("texture2", L"grass.jpg");
    TextureManager::getSingleton()->createTexture(device, L"grass.jpg", L"grass.jpg");

    //mat->setTexture("textureWeights", L"terrainweights.png");
    //TextureManager::getSingleton()->createTexture(device, L"terrainweights.png", L"terrainweights.png");

    
    
    //waterplane
    Dx11Sandbox::string name("waterPlane1");
    m_waterPlane = new WaterPlane(mngr,device, name,D3DXVECTOR3(0,1,0),-60,340,340,200,200);
    
   

}
void DemoApplication::update(SceneManager* mngr,double fTime, float fElapsedTime)
{
    m_time += fElapsedTime;
    handleInput(mngr,fElapsedTime, fTime);

    m_lastMaterial = 0;

}


void DemoApplication::handleInput(SceneManager* mngr, float dt, float elapsedTime)
{
    Camera& cam = mngr->getMainCamera();

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
        cam.moveCameraViewRelative(mov.x, mov.y, mov.z);
    }





    //le mouse
    POINT mousePos;
    D3DXVECTOR2 mousePosVec;



    if(m_moveMouse)
    {
            // Get current position of mouse
        GetCursorPos( &mousePos );
        mousePosVec = D3DXVECTOR2(mousePos.x, mousePos.y);
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
        cam.rotateCameraViewRelative(-m_mouseDelta.y*mouseSens*dt ,-m_mouseDelta.x*mouseSens*dt, 0);
    }

}

void DemoApplication::shutDown(SceneManager* mngr)
{
}


void DemoApplication::renderingBin(std::vector< RenderBinHandler::PRIMITIVETYPE*> &primitives, RenderContext* state)
{
    for( int i = 0; i < primitives.size(); ++i )
    {
        RenderBinHandler::PRIMITIVETYPE* object = primitives[i];
        Material* mat = object->mat;

        //new pass, don't try to skip effect state setting
    

        if(mat==state->getBoundMaterial())
            return;
        m_lastMaterial = mat;

        //D3DXMATRIX *world;
        const D3DXMATRIX *view = m_mngr->getMainCamera().getViewMatrix();
        const D3DXMATRIX *proj = m_mngr->getMainCamera().getProjectionMatrix();
        D3DXMATRIX viewProj =  (*view) * (*proj);

        //temp
        D3DXVECTOR4 sunDir( std::cos( m_time ), 0.f, std::sin( m_time ),0 );
        D3DXVec4Normalize(&sunDir, &sunDir);
        D3DXVECTOR4 sunCol(1.0f,1.f,1.f,0);
        D3DXVECTOR3 transl = -(*m_mngr->getMainCamera().getTranslation());
        D3DXVECTOR4 camPos(transl.x, transl.y, transl.z, 1);

        ID3DX11Effect* effect =  mat->getEffect();
        ID3DX11EffectConstantBuffer* buffer = effect->GetConstantBufferByName("sceneInfo");
        if(buffer->IsValid())
        {
            ID3DX11EffectMatrixVariable* mat =  buffer->GetMemberByName("viewProj")->AsMatrix();
            mat->SetMatrix((float*)&viewProj);
            buffer->GetMemberByName("sunDirection")->AsVector()->SetFloatVector((float*)&sunDir);
            buffer->GetMemberByName("sunColor")->AsVector()->SetFloatVector((float*)&sunCol);
            buffer->GetMemberByName("camPos")->AsVector()->SetFloatVector((float*)&camPos);
            buffer->GetMemberByName("clipPlane")->AsVector()->SetFloatVector((float*)&state->getCustomClipPlane());
            buffer->GetMemberByName("time")->AsScalar()->SetFloat(m_time);
        }
    }
}