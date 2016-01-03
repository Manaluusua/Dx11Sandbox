#ifndef DX11SANDBOX_RENDERDATA_H
#define DX11SANDBOX_RENDERDATA_H

#include "RCObjectPtr.h"
#include "RCObject.h"
#include "Mesh.h"
#include "Material.h"
#include "CommonMath.h"

namespace Dx11Sandbox
{

	class RenderData
	{
	public:
		RenderData();
		~RenderData();
		void setMesh(Mesh* mesh);
		void setMaterial(Material* mat);
		void setWorldMatrix(const Matrix& matrix);
		void setWorldBounds(const Vec4& bounds);
		
		const Matrix& getWorldMatrix() const;
		const Vec4& getWorldBounds() const;
		RCObjectPtr<Material> getMaterial();
		RCObjectPtr<Mesh> getMesh();

	protected:
		Matrix m_worldMatrix;
		Vec4 m_bounds;
		RCObjectPtr<Mesh> m_mesh;
        RCObjectPtr<Material> m_material;
	};



	inline void RenderData::setWorldMatrix(const Matrix& matrix)
	{
		m_worldMatrix = matrix;
	}

	

	inline void RenderData::setMesh(Mesh* m)
	{
		m_mesh = m;
	}

	inline void RenderData::setMaterial(Material* mat)
	{
		m_material = mat;
	}

	inline void RenderData::setWorldBounds(const Vec4& bounds)
	{
		m_bounds = bounds;
	}

	inline const Matrix& RenderData::getWorldMatrix() const
	{
		return m_worldMatrix;
	}


	inline const Vec4& RenderData::getWorldBounds() const
	{
		return m_bounds;
	}

	inline RCObjectPtr<Material> RenderData::getMaterial()
	{
		return m_material;
	}

	inline RCObjectPtr<Mesh> RenderData::getMesh()
	{
		return m_mesh;
	}
	
};

#endif