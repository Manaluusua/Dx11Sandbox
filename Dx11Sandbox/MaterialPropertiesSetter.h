#ifndef DX11SANDBOX_MATERIALPROPERTIESSETTER_H
#define DX11SANDBOX_MATERIALPROPERTIESSETTER_H

namespace Dx11Sandbox
{
	class Material;

	class MaterialPropertiesSetter
	{
	public:

		virtual void SetShaderProperties(Material* mat) = 0;

		virtual ~MaterialPropertiesSetter(void){};
	};

};

#endif