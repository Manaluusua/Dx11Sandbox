#include "InputLayoutDescription.h"
#include <assert.h>
namespace Dx11Sandbox
{
	InputLayoutDescription::InputLayoutDescription()
	{
	}


	InputLayoutDescription::~InputLayoutDescription()
	{
	}

	InputLayoutDescription& InputLayoutDescription::appendDescription(LPCSTR semantic, DXGI_FORMAT format, UINT semanticIndex, UINT slot, UINT byteOffset, D3D11_INPUT_CLASSIFICATION inputClass, UINT instanceStep)
	{
		m_descriptions.push_back({ semantic, semanticIndex, format, slot, byteOffset, inputClass, instanceStep });
		return *this;
	}

	UINT InputLayoutDescription::getElementCount() const
	{
		return m_descriptions.size();
	}
	UINT InputLayoutDescription::getElementByteWidth(UINT index) const
	{
		assert(index < m_descriptions.size());
		if (index >= m_descriptions.size()) return 0;
		DXGI_FORMAT format = m_descriptions[index].Format;
		UINT size = 0;
		switch (format)
		{
			case DXGI_FORMAT_R32G32B32_FLOAT:
				size = 12;
				break;
			case DXGI_FORMAT_R32G32_FLOAT:
				size = 8;
				break;
			case DXGI_FORMAT_R32_FLOAT:
				size = 4;
				break;
			case DXGI_FORMAT_R16_FLOAT:
				size = 2;
				break;
		}
		return size;

	}

	const D3D11_INPUT_ELEMENT_DESC* InputLayoutDescription::getElementDescriptions() const
	{
		return m_descriptions.data();
	}
}
