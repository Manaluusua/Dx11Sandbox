#ifndef DX11SANDBOX_MATERIALMANAGER_H
#define DX11SANDBOX_MATERIALMANAGER_H
#include <map>
#include "CommonUtilities.h"
#include "Singleton.h"
#include "RCObjectPtr.h"
#include <DXUT.h>
#include "ILDescriptions.h"

namespace Dx11Sandbox
{
    class Material;
    typedef std::map<string, Material*>::iterator MaterialMapIterator;
    class MaterialManager : public Singleton<MaterialManager>
    {
		SINGLETON(MaterialManager)
    public:


        virtual ~MaterialManager();

  

        Material* getMaterial(const string& matname);
        bool releaseMaterial(const string& matname);

        bool createMaterial(ID3D11Device* device, const string& filename,const string& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);
        Material* getOrCreateMaterial(ID3D11Device* device, const string& filename,const string& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);

    private:

        MaterialManager();
        DISABLE_COPY(MaterialManager)

        std::map<string, RCObjectPtr<Material> > m_loadedMaterials;
    };
}
#endif