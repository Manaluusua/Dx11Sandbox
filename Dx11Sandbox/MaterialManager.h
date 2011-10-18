#ifndef DX11SANDBOX_MATERIALMANAGER_H
#define DX11SANDBOX_MATERIALMANAGER_H
#include <map>
#include "CommonUtilities.h"
#include <DXUT.h>
#include "ILDescriptions.h"

namespace Dx11Sandbox
{
    class Material;
    typedef std::map<wstring, Material*>::iterator MaterialMapIterator;
    class MaterialManager
    {
    public:
        static MaterialManager* const getSingleton();
        static void destroyMaterialManager();

        virtual ~MaterialManager();

  

        Material* getMaterial(const wstring& matname);
        bool releaseMaterial(const wstring& matname);

        bool createMaterial(ID3D11Device* device, const wstring& filename,const wstring& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);
        Material* getOrCreateMaterial(ID3D11Device* device, const wstring& filename,const wstring& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);

    private:

        MaterialManager();
        DISABLE_COPY(MaterialManager)

        static MaterialManager* m_instance;
        std::map<wstring, Material*> m_loadedMaterials;
    };
}
#endif