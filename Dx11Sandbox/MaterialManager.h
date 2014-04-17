#ifndef DX11SANDBOX_MATERIALMANAGER_H
#define DX11SANDBOX_MATERIALMANAGER_H
#include <map>
#include "CommonUtilities.h"
#include "Singleton.h"
#include "RCObjectPtr.h"
#include <DXUT.h>

namespace Dx11Sandbox
{
    class Material;
	class InputLayoutDescription;
    typedef std::map<string, Material*>::iterator MaterialMapIterator;
    class MaterialManager : public Singleton<MaterialManager>
    {
		SINGLETON(MaterialManager)
    public:


        virtual ~MaterialManager();

		void setAssetPath(const string& path);
		const string& getAssetPath() const;

        Material* getMaterial(const string& matname);
        bool releaseMaterial(const string& matname);

		bool createMaterial(ID3D11Device* device, const string& filename, const string& matname, const InputLayoutDescription& inputDescription);
		Material* getOrCreateMaterial(ID3D11Device* device, const string& filename, const string& matname, const InputLayoutDescription& inputDescription);

    private:

        MaterialManager();
        DISABLE_COPY(MaterialManager)

		string m_assetPath;
        std::map<string, RCObjectPtr<Material> > m_loadedMaterials;
    };
}
#endif