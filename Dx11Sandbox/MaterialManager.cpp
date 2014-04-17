#include "MaterialManager.h"
#include "Material.h"
#include "InputLayoutDescription.h"
namespace Dx11Sandbox
{

    MaterialManager::MaterialManager()
    {
    }


    MaterialManager::~MaterialManager()
    {
        m_loadedMaterials.clear();
            
    }
	const string& MaterialManager::getAssetPath() const
	{
		return m_assetPath;
	}

	bool MaterialManager::createMaterial(ID3D11Device* device, const string& filename, const string& matname, const InputLayoutDescription& inputDescription)
    {
        



        assert(m_loadedMaterials.find(matname)==m_loadedMaterials.end());
        if (m_loadedMaterials.find(matname)!=m_loadedMaterials.end())
        {
            return false;
        }

        Material* mat = new Material();

		string path = m_assetPath + filename;

        if(mat->loadAndInitializeMaterial(path,device,inputDescription))
        {
            m_loadedMaterials[matname] = mat;
            return true;
        }else
        {
            delete mat;
            return false;
        }
    }

	void MaterialManager::setAssetPath(const string& path)
	{
		m_assetPath = path;
	}

	Material* MaterialManager::getOrCreateMaterial(ID3D11Device* device, const string& filename, const string& matname, const InputLayoutDescription& inputDescription)
    {
        Material* mat = getMaterial(matname);
        if(!mat)
        {
            if(createMaterial(device,filename, matname, inputDescription))
            {
                mat = getMaterial(matname);
            }
        }
        return mat;
    }

    bool MaterialManager::releaseMaterial(const string& matname)
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

    Material* MaterialManager::getMaterial(const string& matname)
    {
        if(m_loadedMaterials.find(matname)!=m_loadedMaterials.end())
            return m_loadedMaterials.at(matname);
        else
            return 0;
    }



}