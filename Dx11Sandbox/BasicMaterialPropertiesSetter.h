#ifndef DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H
#define DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H

#include "MaterialPropertiesSetter.h"

namespace Dx11Sandbox
{
	class RenderCamera;

	class BasicMaterialPropertiesSetter : public MaterialPropertiesSetter
	{
	public:
		BasicMaterialPropertiesSetter();
		virtual ~BasicMaterialPropertiesSetter(void);

		virtual void SetCurrentCamera(RenderCamera* cam);

		virtual void SetShaderProperties(Material* mat);


	protected:
		RenderCamera* m_cam;
		Material* m_previousMaterial;
	};

};

#endif