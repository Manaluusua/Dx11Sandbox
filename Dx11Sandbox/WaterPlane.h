#ifndef WATERPLANE_H
#define WATERPLANE_H

#include "SceneManager.h"
#include "CommonUtilities.h"
#include "RCObject.h"


namespace Dx11Sandbox
{
    class Mesh;
    class Texture;
    class CullInfo;
}

#define WATERPLANE_WAVECOUNT 3

class WaterPlane: public Dx11Sandbox::RCObject, public Dx11Sandbox::RenderStartListener
{
public:


    struct WaveDefinition
    {
        WaveDefinition();
        WaveDefinition(D3DXVECTOR2 dir,float amplitude, float waveLength, float speed);
        D3DXVECTOR2 direction;
        float amplitude;
        float frequency;
        float phaseConstant;
    };

    WaterPlane(Dx11Sandbox::SceneManager* mngr, ID3D11Device *device, const Dx11Sandbox::string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2, float tesselationX = 5, float tesselationY = 5);
    ~WaterPlane(void);

    virtual void renderingStarted(Dx11Sandbox::RenderContext* context,Dx11Sandbox::SceneManager* mngr, double fTime, float fElapsedTime);

private:
    void setupWaves();

    WaveDefinition m_waves[WATERPLANE_WAVECOUNT];
    D3DXVECTOR3 m_normal;
    Dx11Sandbox::CullInfo** m_renderObject;
    Dx11Sandbox::Texture* m_reflection;
    Dx11Sandbox::Texture* m_refraction;
    Dx11Sandbox::Texture* m_normalMap;
    float m_d;
};

#endif
