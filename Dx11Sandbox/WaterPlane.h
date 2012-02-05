#ifndef WATERPLANE_H
#define WATERPLANE_H

#include "SceneManager.h"
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
    class Mesh;
    class Texture;
}



class WaterPlane: public Dx11Sandbox::RenderStartListener
{
public:
    WaterPlane(Dx11Sandbox::SceneManager* mngr, ID3D11Device *device, const Dx11Sandbox::string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2);
    ~WaterPlane(void);


    virtual void renderingStarted(Dx11Sandbox::RenderContext* context,Dx11Sandbox::SceneManager* mngr, double fTime, float fElapsedTime);

private:
    Dx11Sandbox::Mesh* m_plane;
    Dx11Sandbox::Texture* m_reflection;
    Dx11Sandbox::Texture* m_refraction;
    Dx11Sandbox::Material* m_mat;
    D3DXVECTOR3 m_normal;
    float m_d;
};

#endif
