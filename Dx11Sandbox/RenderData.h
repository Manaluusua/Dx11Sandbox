#ifndef DX11SANDBOX_RENDERDATA_H
#define DX11SANDBOX_RENDERDATA_H

#include "RCObjectPtr.h"

namespace Dx11Sandbox
{
	class Mesh;
	class Material;

	class RenderData
	{
	public:
		RenderData();
		virtual ~RenderData();
		void setMesh(Mesh* mesh);
		void setMaterial(Material* mat);

		RCObjectPtr<Material> getMaterial();
		RCObjectPtr<Mesh> getMesh();

	protected:
		RCObjectPtr<Mesh> m_mesh;
        RCObjectPtr<Material> m_material;
	};


	
	
};

#endif