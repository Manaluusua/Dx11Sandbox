#include "MaterialManager.h"
#include "Material.h"
namespace Dx11Sandbox
{

    MaterialManager::MaterialManager()
    {
    }


    MaterialManager::~MaterialManager()
    {
        m_loadedMaterials.clear();
            
    }

    bool MaterialManager::createMaterial(ID3D11Device* device, const wstring& filename,const wstring& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        



        assert(m_loadedMaterials.find(matname)==m_loadedMaterials.end());
        if (m_loadedMaterials.find(matname)!=m_loadedMaterials.end())
        {
            return false;
        }

        Material* mat = new Material();

        if(mat->loadAndInitializeMaterial(filename,device,type))
        {
            m_loadedMaterials[matname] = mat;
            return true;
        }else
        {
            delete mat;
            return false;
        }
    }


    Material* MaterialManager::getOrCreateMaterial(ID3D11Device* device, const wstring& filename,const wstring& matname, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        Material* mat = getMaterial(matname);
        if(!mat)
        {
            if(createMaterial(device,filename, matname, type))
            {
                mat = getMaterial(matname);
            }
        }
        return mat;
    }

    bool MaterialManager::releaseMaterial(const wstring& matname)
    {
        Material* mat = getMaterial(matname);
        if(mat)
        {
            m_loadedMaterials.erase(matname);
            return true;
        }else
        {
            return false;
        }
    }

    Material* MaterialManager::getMaterial(const wstring& matname)
    {
        if(m_loadedMaterials.find(matname)!=m_loadedMaterials.end())
            return m_loadedMaterials.at(matname);
        else
            return 0;
    }



}