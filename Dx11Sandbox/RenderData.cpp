#include "RenderData.h"
#include "Mesh.h"
#include "Material.h"

namespace Dx11Sandbox
{

	RenderData::RenderData(void)
		:m_mesh(0),
		m_material(0)
	{
	}


	RenderData::~RenderData(void)
	{
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