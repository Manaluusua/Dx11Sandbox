#ifndef DX11SANDBOX_LIGHTLISTTOSTRUCTUREDDATA_H
#define DX11SANDBOX_LIGHTLISTTOSTRUCTUREDDATA_H

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
	class RenderContext;

	class LightListToStructuredData
	{
	public:
		LightListToStructuredData();
		void setupFromLightList(RenderContext* rc, std::vector<Light*>* lights);
		ID3D11ShaderResourceView* getResourceViewOfLightData();


		~LightListToStructuredData();

	private:

		void createBuffer(RenderContext* rc,UINT structSize, UINT structCount);
		void copyLightsToBuffer(RenderContext* rc, std::vector<Light*>* lights);

		RCObjectPtr<GPUBuffer> m_buffer;
		ReleasePtr<ID3D11ShaderResourceView> m_resourceView;
	};

}

#endif
