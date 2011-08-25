#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H
#include "CommonUtilities.h"
#include "Mesh.h"
class CBaseCamera;

namespace Dx11Sandbox
{
    class Material;
    

    class RenderObject
    {
        Material* m_material;
        Mesh* m_mesh;

    public:
        Material* const getMaterial(){return m_material;}
        Mesh* const getMesh(){return m_mesh;}

        RenderObject(Mesh* mesh, Material* material);
        ~RenderObject();

        void render(ID3D11Device* pd3dDevice, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat=0);
    };

}



#endif