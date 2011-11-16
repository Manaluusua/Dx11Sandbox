#include "DemoApplication.h"
#include "SceneManager.h"
#include "MeshUtility.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "MeshManager.h"
#include "RenderObject.h"

void DemoApplication::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown)
{

}

void DemoApplication::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                    bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                    int xPos, int yPos )
{
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

}
void DemoApplication::shutDown(SceneManager* mngr)
{

}
