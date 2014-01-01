#ifndef DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H
#define DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H

#include "MaterialPropertiesSetter.h"
#include <vector>

namespace Dx11Sandbox
{
	class Light;
	class RenderCamera;

	class BasicMaterialPropertiesSetter : public MaterialPropertiesSetter
	{
	public:
		BasicMaterialPropertiesSetter();
		virtual ~BasicMaterialPropertiesSetter(void);

		virtual void setCurrentCamera(RenderCamera* cam);
		virtual void setLights(std::vector<Light*>* lights);

		virtual void setShaderProperties(Material* mat);


	protected:

		Light& getLight();

		RenderCamera* m_cam;
		Material* m_previousMaterial;
		std::vector<Light*>* m_lights;
	};

};

#endif