/*#ifndef DX11SANDBOX_BASICRENDEROBJECT_H
#define DX11SANDBOX_BASICRENDEROBJECT_H


#include "CommonUtilities.h"
#include "RenderObject.h"
class CBaseCamera;

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dx11Sandbox
{
    class Material;
    class Mesh;

    class BasicRenderObject : public RenderObject
    {
        Material* m_material;
        Mesh* m_mesh;


    public:
        virtual Material* const getMaterial(){return m_material;}
        virtual Mesh* const getMesh(){return m_mesh;}
        virtual void setMaterial(Material* mat);
        virtual void setMesh(Mesh* mesh);

        BasicRenderObject(Mesh* mesh, Material* material);
        BasicRenderObject();
        virtual ~BasicRenderObject();

        void render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat=0);
    };

}



#endif*/