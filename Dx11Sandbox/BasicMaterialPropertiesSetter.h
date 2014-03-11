#ifndef DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H
#define DX11SANDBOX_BASICMATERIALPROPERTIESSETTER_H

#include <vector>

namespace Dx11Sandbox
{
	class Light;
	class Camera;
	class RenderData;
	class Material;

	class BasicMaterialPropertiesSetter 
	{
	public:
		BasicMaterialPropertiesSetter();
		virtual ~BasicMaterialPropertiesSetter(void);

		virtual void setCurrentCamera(Camera* cam);
		virtual void setLights(std::vector<Light*>* lights);

		
		void setShaderUniforms(RenderData* object, Material* mat, bool updateSceneUniforms = true, bool updateLightUniforms= true);
	private:

		void setSceneInfoUniforms(RenderData* object, Material* mat);
		void setLightUniforms(Material* mat);

		Light& getLight();

		Camera* m_cam;
		Material* m_previousMaterial;
		std::vector<Light*>* m_lights;
	};

};

#endif