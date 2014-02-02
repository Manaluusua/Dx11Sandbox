#ifndef DX11SANDBOX_RENDERDATA_H
#define DX11SANDBOX_RENDERDATA_H

#include "RCObjectPtr.h"
#include <d3dx9math.h>

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

		virtual void setWorldMatrix(const D3DXMATRIX& matrix);
		virtual const D3DXMATRIX& getWorldMatrix() const;

		RCObjectPtr<Material> getMaterial();
		RCObjectPtr<Mesh> getMesh();

	protected:
		D3DXMATRIX m_worldMatrix;
		RCObjectPtr<Mesh> m_mesh;
        RCObjectPtr<Material> m_material;
	};


	
	
};

#endif