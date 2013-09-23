#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include "RenderData.h"
#include "CullInfo.h"
#include "CommonUtilities.h"


namespace Dx11Sandbox
{
	class CullInfo;
	class Mesh;
	class Material;
	class RenderObjectManager;

	class RenderObject : public RenderData
	{

		friend class RenderObjectManager;
	public:
		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        void setBinFlags(int flags);
        void setCullInfoFlags(CullInfo::CULLINFO_FLAGS flags);

		void setVisible(bool value);

		void destroy();

	protected:
		RenderObject();
		virtual ~RenderObject();

		DISABLE_COPY(RenderObject);
		

		CullInfo** m_cullingInformation;
		RenderObjectManager* m_mngr;
	};


	
	
};

#endif