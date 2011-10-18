#ifndef DX11SANDBOX_RENDERSTATE_H
#define DX11SANDBOX_RENDERSTATE_H

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dx11Sandbox
{
    class Mesh;
    class Material;
    class RenderContext
    {
    public:
        RenderContext(void);
        ~RenderContext(void);

        ID3D11Device* getDevice(){return m_device;}
        void setDevice(ID3D11Device* device){m_device = device;}

        ID3D11DeviceContext* getImmediateContext(){return m_imContext;}
        void setImmediateContext(ID3D11DeviceContext* imContext){m_imContext = imContext;}

        Mesh* getBoundMesh(){return m_boundMesh;}
        Material* getBoundMaterial(){return m_boundMaterial;}

        void bindMesh(Mesh* mesh);
        void bindMaterial(Material* mat);

        void clearState();

    private:

        Mesh* m_boundMesh;
        Material *m_boundMaterial;

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_imContext;


    };
}

#endif