#ifndef DX11SANDBOX_INDEXBUFFER_H
#define DX11SANDBOX_INDEXBUFFER_H

#include "RCObjectPtr.h"
#include "ReleasePtr.h"
#include <d3dx9math.h>
#include <vector>
#include <minwindef.h>
struct ID3D11ShaderResourceView;

namespace Dx11Sandbox
{

	class Light;
	class GPUBuffer;
	class LightListToStructuredData
	{
	public:
		LightListToStructuredData();
		void setupFromLightList(std::vector<Light*>* lights);
		ID3D11ShaderResourceView* getResourceViewOfLightData();


		~LightListToStructuredData();

	private:

		static const UINT s_lightTypeOmni;
		static const UINT s_lightTypeDirectional;

		struct ShaderLightStruct {
			UINT lightType;
			D3DXVECTOR3 pos; //float sqrRadius; 
			D3DXVECTOR3 color; //float invSqrRadius; 
		};

		RCObjectPtr<GPUBuffer> m_buffer;
		ReleasePtr<ID3D11ShaderResourceView> m_resourceView;
	};

}

#endif
