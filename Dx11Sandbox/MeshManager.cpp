#include "MeshManager.h"
#include "Mesh.h"
namespace Dx11Sandbox
{
    MeshManager* MeshManager::m_instance = 0;

    MeshManager::MeshManager()
    {
    }


    MeshManager::~MeshManager()
    {
        MeshMapIterator it = m_loadedMeshes.begin();
        while(it != m_loadedMeshes.end())
        {
            SAFE_DELETE(it->second);
            ++it;
        }
        m_loadedMeshes.clear();
            
    }

    
    Mesh* MeshManager::getMesh(const string& meshname)
    {
        if(m_loadedMeshes.find(meshname)!=m_loadedMeshes.end())
            return m_loadedMeshes.at(meshname);
        else
            return 0;
    }

    Mesh* MeshManager::createMesh(const string& meshname)
    {
         if(m_loadedMeshes.find(meshname)!=m_loadedMeshes.end())
         {
             return 0;
         }
         Mesh* mesh = new Mesh;
         m_loadedMeshes[meshname] = mesh;
         return mesh;
    }

    void MeshManager::destroyMesh(const string& meshname)
    {
        auto iter = m_loadedMeshes.find(meshname);
        if(iter != m_loadedMeshes.end())
        {
            SAFE_DELETE(iter->second);
            m_loadedMeshes.erase( iter );
        }
    }

    Mesh* MeshManager::getOrCreateMesh(const string& meshname)
    {
        Mesh* mesh = getMesh(meshname);
        if(!mesh)
        {
            mesh = createMesh(meshname);
        }
        return mesh;
    }

    MeshManager* const MeshManager::getSingleton()
    {
        if(!m_instance)
        {
            m_instance = new MeshManager();
        }
        return m_instance;
    }


    void MeshManager::destroyMeshManager()
    {
        SAFE_DELETE(m_instance);
    }
}