#ifndef DX11SANDBOX_CULLABLEGEOMETRY_H
#define DX11SANDBOX_CULLABLEGEOMETRY_H

#include "BasicGeometry.h"
#include "Cullable.h"

#include <d3dx9math.h>

namespace Dx11Sandbox
{

	class CullableObjectManager;
	class CullData;
	class Mesh;
	class Material;


	class CullableGeometry : public BasicGeometry, public Cullable
	{
		
		friend class CullableObjectManager;

	public:

		virtual void setWorldMatrix(const D3DXMATRIX& matrix);

		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        virtual void setRenderMask(RenderLayer mask);

		void setVisible(bool value);

		virtual void passedCulling(RenderPrimitiveGatherer *gatherer);
		

		void destroy();

	protected:

		CullableGeometry(CullableObjectManager* mngr);
		virtual ~CullableGeometry();

		void acquireCullData();
		void returnCullData();
		void updateCullData();

		float calculateWorldSpaceRadius(float localRadius);

		DISABLE_COPY(CullableGeometry);

		D3DXVECTOR4 m_bounds;
		CullData** m_cullingInformation;
		CullableObjectManager* m_mngr;
		
		
	};


	
	
};

#endif