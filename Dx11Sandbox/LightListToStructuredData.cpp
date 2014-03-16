#include "LightListToStructuredData.h"
#include "GPUBuffer.h"
#include <D3D11.h>


namespace Dx11Sandbox
{
	const UINT LightListToStructuredData::s_lightTypeOmni = 0;
	const UINT LightListToStructuredData::s_lightTypeDirectional = 1;

	LightListToStructuredData::LightListToStructuredData()
	{
	}


	LightListToStructuredData::~LightListToStructuredData()
	{
	}

	void LightListToStructuredData::setupFromLightList(std::vector<Light*>* lights)
	{

	}
	ID3D11ShaderResourceView* LightListToStructuredData::getResourceViewOfLightData()
	{
		return m_resourceView;
	}


}

