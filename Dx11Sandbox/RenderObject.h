#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include <d3dx9math.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
class CBaseCamera;
namespace Dx11Sandbox
{
    class Material;
    class RenderObject
    {
    public:
        virtual ~RenderObject();

        virtual D3DXMATRIX& getWorldTransformation(){return m_world;};

        virtual UINT32 getRenderObjectMask(){return m_renderObjectMask;}
        virtual void setRenderObjectMask(UINT32 mask){m_renderObjectMask=mask;}

        virtual void render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat=0) = 0;
    protected:

        RenderObject();
        

        UINT32 m_renderObjectMask;
        D3DXMATRIX m_world;
    };
}
#endif