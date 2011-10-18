/*#ifndef DX11SANDBOX_COMPOSITEOBJECT_H
#define DX11SANDBOX_COMPOSITEOBJECT_H

#include "renderobject.h"
#include <vector>


namespace Dx11Sandbox
{
    class Mesh;
    class CompositeObject :
        public RenderObject
    {

    public:


        virtual ~CompositeObject(void);

        void setIndexBuffers( Mesh* indexes,  unsigned int numObj);
        void setVertexBuffers( Mesh* vertices);
        void setMaterial(Material* mat){m_material = mat;};
        unsigned int renderObjectCount(){return m_subObjectCount;}

        CompositeObject();

        void render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat=0);

    private:
        Mesh* m_sharedVertexBuffer;
        Mesh* m_indexData;
        Material* m_material;
        unsigned int m_subObjectCount;

    };
}

#endif*/