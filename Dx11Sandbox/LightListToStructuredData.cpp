#include "LightListToStructuredData.h"
#include "GPUBuffer.h"
#include "RenderContext.h"
#include "Light.h"
#include <D3D11.h>


namespace Dx11Sandbox
{

	LightListToStructuredData::LightListToStructuredData()
		:m_buffer(new GPUBuffer())
	{
	}


	LightListToStructuredData::~LightListToStructuredData()
	{
	}

	void LightListToStructuredData::setupFromLightList(RenderContext* rc, std::vector<Light*>* lights)
	{
		if (!lights || !rc) return;
		UINT lightCount = lights->size();
		UINT lightStructSize = sizeof(LightDefinition);
		UINT byteCount = lightCount * lightStructSize;

		if (m_buffer->getByteCount() < byteCount){
			createBuffer(rc,lightStructSize, lightCount);
		}

		copyLightsToBuffer(rc, lights);
	}

	void LightListToStructuredData::createBuffer(RenderContext* rc, UINT structSize, UINT structCount)
	{
		m_buffer->allocateBuffer(rc->getDevice(), 0, structSize * structCount, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, structSize);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.ElementOffset = 0;
		srvDesc.Buffer.ElementWidth = structSize;
		srvDesc.Buffer.NumElements = structCount;

		ID3D11ShaderResourceView* srv;
		rc->getDevice()->CreateShaderResourceView(m_buffer->getBuffer(), &srvDesc, &srv);
		m_resourceView = srv;
		
		//decrease count by one
		srv->Release();
	}
	void LightListToStructuredData::copyLightsToBuffer(RenderContext* rc, std::vector<Light*>* lights)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hResult = rc->getImmediateContext()->Map(m_buffer->getBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (hResult != S_OK)
		{
			showErrorDialog("LightListToStructuredData::copyLightsToBuffer: Failed to map Buffer for copying");
			return;
		}

		char* dataPtr = reinterpret_cast<char*>(mappedResource.pData);
		UINT structSize = sizeof(LightDefinition);

		for (unsigned int i = 0; i < lights->size(); ++i){
			Light* l = (*lights)[i];
			const LightDefinition& lightDef = l->getLightDefinition();
			memcpy(dataPtr, &lightDef, structSize);
			dataPtr += structSize;
		}

		rc->getImmediateContext()->Unmap(m_buffer->getBuffer(), 0);
	}

	ID3D11ShaderResourceView* LightListToStructuredData::getResourceViewOfLightData()
	{
		return m_resourceView;
	}


}

