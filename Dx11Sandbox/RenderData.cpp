#include "RenderData.h"
#include "Mesh.h"
#include "Material.h"

namespace Dx11Sandbox
{

	RenderData::RenderData(void)
		:m_mesh(0),
		m_material(0)
	{
		D3DXMatrixIdentity(&m_worldMatrix);
	}


	RenderData::~RenderData(void)
	{
	}


	void RenderData::setWorldMatrix(const D3DXMATRIX& matrix)
	{
		m_worldMatrix = matrix;
	}

	const D3DXMATRIX& RenderData::getWorldMatrix() const
	{
		return m_worldMatrix;
	}


	void RenderData::setMesh(Mesh* m)
	{
		m_mesh = m;
	}

	void RenderData::setMaterial(Material* mat)
	{
		m_material = mat;
	}

	RCObjectPtr<Material> RenderData::getMaterial()
	{
		return m_material;
	}

	RCObjectPtr<Mesh> RenderData::getMesh()
	{
		return m_mesh;
	}

}