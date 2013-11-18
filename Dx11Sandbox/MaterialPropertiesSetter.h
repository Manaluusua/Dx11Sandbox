#ifndef DX11SANDBOX_MATERIALPROPERTIESSETTER_H
#define DX11SANDBOX_MATERIALPROPERTIESSETTER_H

namespace Dx11Sandbox
{
	class Material;
	class MaterialPropertiesSetter
	{
		friend class SceneManager;
	public:

		virtual void setShaderProperties(Material* mat) = 0;

		virtual ~MaterialPropertiesSetter(void){};

		

	

	};


	


};

#endif