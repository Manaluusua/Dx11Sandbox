#ifndef DX11SANDBOX_MESHMANAGER_H
#define DX11SANDBOX_MESHMANAGER_H

#include "CommonUtilities.h"
#include "RCObjectPtr.h"
#include <map>

namespace Dx11Sandbox
{
    class Mesh;
    typedef std::map<string, RCObjectPtr< Mesh > >::iterator MeshMapIterator;
    class MeshManager
    {
    public:

        static MeshManager* const getSingleton();
        static void destroyMeshManager();
        
        Mesh* getMesh(const string& meshname);
        Mesh* createMesh(const string& meshname);
        void destroyMesh(const string& meshname);
        Mesh* getOrCreateMesh(const string& meshname);
        ~MeshManager();
    private:
        MeshManager();
        DISABLE_COPY(MeshManager)
        static MeshManager* m_instance;
        std::map<string, RCObjectPtr< Mesh > > m_loadedMeshes;
    };
}

#endif