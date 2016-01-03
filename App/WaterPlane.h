#ifndef WATERPLANE_H
#define WATERPLANE_H

#include "SceneManager.h"
#include "RenderCamera.h"
#include "CommonUtilities.h"
#include "CommonMath.h"
#include "RCObject.h"


namespace Dx11Sandbox
{
    class Mesh;
    class Texture;
	class Material;
	class RenderContext;
}

#define WATERPLANE_WAVECOUNT 3

class WaterPlane: public Dx11Sandbox::RCObject, public Dx11Sandbox::RenderCameraListener
{
public:


    struct WaveDefinition
    {
        WaveDefinition();
        WaveDefinition(Dx11Sandbox::Vec2 dir,float amplitude, float waveLength, float speed);
		Dx11Sandbox::Vec2 direction;
        float amplitude;
        float frequency;
        float phaseConstant;
    };

	WaterPlane(Dx11Sandbox::SceneManager* mngr, ID3D11Device *device, const Dx11Sandbox::string& name, Dx11Sandbox::Vec3 normal, float d, float extends1, float extends2, float tesselationX, float tesselationY, int textureResolution);
    ~WaterPlane(void);

	virtual void cameraStartedRendering(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderBin& renderbin, Dx11Sandbox::RenderContext* state); 

	virtual void update(double fTime, float fElapsedTime);

	virtual void cameraPreCull(Dx11Sandbox::RenderCamera& camera);
	virtual void cameraEndedRendering(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderBin& renderbin, Dx11Sandbox::RenderContext* state);

	Dx11Sandbox::Texture* getReflectionTexture();
	Dx11Sandbox::Texture* getRefractionTexture();

private:
    void setupWaves();

	void initializeRenderTargets(ID3D11Device *device, Dx11Sandbox::RCObjectPtr<Dx11Sandbox::Material> mat, const Dx11Sandbox::string& name, int textureResolution);

	void setupReflectionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state);
	void cleanupReflectionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state);

	void setupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state);
	void cleanupRefractionCamera(Dx11Sandbox::RenderCamera& camera, Dx11Sandbox::RenderContext* state);

    WaveDefinition m_waves[WATERPLANE_WAVECOUNT];
	Dx11Sandbox::Vec3 m_normal;

	Dx11Sandbox::RenderCamera* m_reflectionCamera;
	Dx11Sandbox::RenderCamera* m_refractionCamera;

    Dx11Sandbox::CullableGeometry* m_renderObject;
    Dx11Sandbox::Texture* m_reflection;
    Dx11Sandbox::Texture* m_refraction;
	Dx11Sandbox::Texture* m_depthStencil;
    Dx11Sandbox::Texture* m_normalMap;

	D3D11_VIEWPORT waterplaneViewport;

	Dx11Sandbox::SceneManager* m_mngr;

    float m_d;
};

#endif
