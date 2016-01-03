#ifndef DX11SANDBOX_INPUTLAYOUTDESCRIPTION_H
#define DX11SANDBOX_INPUTLAYOUTDESCRIPTION_H

#include <stdint.h>
#include <D3D11.h>
#include <vector>

namespace Dx11Sandbox
{
	class InputLayoutDescription
	{
	public:
		InputLayoutDescription();
		~InputLayoutDescription();

		InputLayoutDescription& appendDescription(LPCSTR semantic, DXGI_FORMAT format, uint32_t semanticIndex = 0, uint32_t slot = 0, uint32_t byteOffset = D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION inputClass = D3D11_INPUT_PER_VERTEX_DATA, uint32_t instanceStep = 0);

		uint32_t getElementCount() const;
		uint32_t getElementByteWidth(uint32_t index) const;
		const D3D11_INPUT_ELEMENT_DESC* getElementDescriptions() const;

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_descriptions;
	};

};

#endif

