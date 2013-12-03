#ifndef DX11SANDBOX_CULLABLEGEOMETRY_H
#define DX11SANDBOX_CULLABLEGEOMETRY_H

#include "Geometry.h"
#include "Cullable.h"

#include <d3dx9math.h>

namespace Dx11Sandbox
{
	template<class T>
	class CullableObjectManager;
	class CullableGeometry;
	class CullData;
	class Mesh;
	class Material;

	typedef CullableObjectManager<CullableGeometry> CullableGeometryManager;

	class CullableGeometry : public Geometry, public Cullable
	{
		
		friend class CullableObjectManager<CullableGeometry>;

	public:

		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        virtual void setRenderMask(RenderLayer mask);

		void setVisible(bool value);

		virtual CullableType GetCullableType() const;

		

		void destroy();

	protected:

		CullableGeometry(CullableGeometryManager* mngr);
		virtual ~CullableGeometry();

		void acquireCullData();
		void returnCullData();
		void updateCullData();

		DISABLE_COPY(CullableGeometry);

		D3DXVECTOR4 m_bounds;
		CullData** m_cullingInformation;
		CullableGeometryManager* m_mngr;
		
		
	};


	
	
};

#endif