#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include "RCObjectPtr.h"
#include "CullInfo.h"
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
	class CullInfo;
	class Mesh;
	class Material;
	class RenderObjectManager;

	class RenderObject
	{

		friend class RenderObjectManager;
	public:
		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        void setBinFlags(int flags);
        void setCullInfoFlags(CullInfo::CULLINFO_FLAGS flags);
		void setMesh(Mesh* mesh);
		void setMaterial(Material* mat);

		void destroy();

		RCObjectPtr<Material> getMaterial();
		RCObjectPtr<Mesh> getMesh();

	private:

		DISABLE_COPY(RenderObject)
		RenderObject();
		~RenderObject();

		CullInfo** m_cullingInformation;
		RCObjectPtr<Mesh> m_mesh;
        RCObjectPtr<Material> m_material;
		RenderObjectManager* m_mngr;
	};


	
	
};

#endif