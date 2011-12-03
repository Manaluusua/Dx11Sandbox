#include "DemoApplication.h"
#include "SceneManager.h"
#include "MeshUtility.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "MeshManager.h"
#include "RenderObject.h"


DemoApplication::DemoApplication()
    :m_leftDown(false),
    m_rightDown(false),
    m_forwardDown(false),
    m_backwardDown(false),
    m_upDown(false),
    m_downDown(false),
    m_moveMouse(false),
    m_lastMousePos(-1,-1),
    m_mouseDelta(0,0)
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
void DemoApplication::createWorld(SceneManager* mngr)
{
    
    
    Camera& cam = mngr->getMainCamera();
    ID3D11Device* device = mngr->getDevice();
    //camera
    D3DXVECTOR3 vecEye( 0.0f, 15.0f, -0.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -15.0f );
    D3DXVECTOR3 up (0.0f,1.0f,0.0f);
    cam.lookAt(vecEye, vecAt, up);
        


        
    //objects
    Material* mat = 0; 
    Mesh* mesh = 0; 
    RenderObject **ro;

    //skybox
    mat = MaterialManager::getSingleton()->getOrCreateMaterial(device, L"skybox.fx", L"skybox",MeshInputLayouts::POS3TEX3);
    mesh = MeshUtility::createSkyBoxMesh(device, "skybox" + generateID());
    ro = mngr->allocateDynamic();
    (*ro)->mat = mat;
    (*ro)->mesh = mesh;
    (*ro)->renderQueueFlag = RenderQueueFlag::RFINAL;

    mat->setTexture("cubemap", L"skyboxCube.dds");
    TextureManager::getSingleton()->createTexture(device, L"skyboxCube.dds", L"skyboxCube.dds");
     
    //terrain
    mat = MaterialManager::getSingleton()->getOrCreateMaterial(device, L"terrain.fx", L"terrain1",MeshInputLayouts::POS3NORM3TEX2);
    mat->setTexture("texture1", L"grass.jpg");
    TextureManager::getSingleton()->createTexture(device, L"grass.jpg", L"grass.jpg");
    MeshUtility::createTerrainFromHeightMap(device,mngr, L"heightmapTerrain.png", mat,500,500,50,30,30,20);

}
void DemoApplication::update(SceneManager* mngr,double fTime, float fElapsedTime)
{

  handleInput(mngr,fElapsedTime, fTime);


}


void DemoApplication::handleInput(SceneManager* mngr, float dt, float elapsedTime)
{
    Camera& cam = mngr->getMainCamera();

    //cam movement
    //keyboard
    D3DXVECTOR3 mov(0,0,0);
    float speed = 40;
    float mouseSens = 0.3;

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
