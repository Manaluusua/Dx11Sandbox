#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include "CommonUtilities.h"
class CBaseCamera;

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dx11Sandbox
{
    class Material;
    class Mesh;

    class RenderObject
    {
        Material* m_material;
        Mesh* m_mesh;
        UINT32 m_renderObjectMask;

    public:
        virtual Material* const getMaterial(){return m_material;}
        virtual Mesh* const getMesh(){return m_mesh;}

        UINT32 getRenderObjectMask(){return m_renderObjectMask;}
        void setRenderObjectMask(UINT32 mask){m_renderObjectMask=mask;}

        RenderObject(Mesh* mesh, Material* material);
        virtual ~RenderObject();

        void render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat=0);
    };

}



#endif