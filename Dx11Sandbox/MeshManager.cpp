#include "MeshManager.h"
#include "Mesh.h"
namespace Dx11Sandbox
{

    MeshManager::MeshManager()
    {
    }


    MeshManager::~MeshManager()
    {
       
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

}