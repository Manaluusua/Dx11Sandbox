#include "RenderObject.h"
#include "CullInfoManager.h"
#include "RenderObjectManager.h"
#include "Mesh.h"
#include "Material.h"
namespace Dx11Sandbox
{
	RenderObject::RenderObject(void)
		:m_mesh(0),
		m_material(0),
		m_mngr(0)
	{
		m_cullingInformation = CullInfoManager::singleton()->allocate();
		(*m_cullingInformation)->object = this;
	}


	RenderObject::~RenderObject(void)
	{
		CullInfoManager::singleton()->deallocate(m_cullingInformation);
	}

	void RenderObject::destroy()
	{
		m_mngr->DestroyRenderObject(this);
	}

	void RenderObject::setBoundingSphere(const D3DXVECTOR4& bounds)
	{
		(*m_cullingInformation)->boundingSphere = bounds;
	}

    void RenderObject::setBinFlags(int flags)
	{
		(*m_cullingInformation)->binIDFlag = flags;
	}
    void RenderObject::setCullInfoFlags(CullInfo::CULLINFO_FLAGS flags)
	{
		(*m_cullingInformation)->flags = flags;
	}
	
	void RenderObject::setMesh(Mesh* m)
	{
		m_mesh = m;
	}

	void RenderObject::setMaterial(Material* mat)
	{
		m_material = mat;
	}

	RCObjectPtr<Material> RenderObject::getMaterial()
	{
		return m_material;
	}

	RCObjectPtr<Mesh> RenderObject::getMesh()
	{
		return m_mesh;
	}
}