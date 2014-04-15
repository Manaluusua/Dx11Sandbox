#ifndef DX11SANDBOX_INPUTLAYOUTDESCRIPTION_H
#define DX11SANDBOX_INPUTLAYOUTDESCRIPTION_H

#include <D3D11.h>
#include <vector>

namespace Dx11Sandbox
{
	class InputLayoutDescription
	{
	public:
		InputLayoutDescription();
		~InputLayoutDescription();

		InputLayoutDescription& appendDescription(LPCSTR semantic, DXGI_FORMAT format, UINT semanticIndex = 0, UINT slot = 0, UINT byteOffset = D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION inputClass = D3D11_INPUT_PER_VERTEX_DATA, UINT instanceStep = 0);

		UINT getElementCount() const;
		UINT getElementByteWidth(UINT index) const;
		const D3D11_INPUT_ELEMENT_DESC* getElementDescriptions() const;

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_descriptions;
	};

};

#endif

