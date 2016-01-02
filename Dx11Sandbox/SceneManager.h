#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include <vector>

struct ID3D11Device;
struct IDXGISwapChain;
struct DXGI_SURFACE_DESC;
struct ID3D11DeviceContext;
namespace Dx11Sandbox
{
	class Root;
	class RenderCamera;
	class RenderContext;
	class RenderBin;
	class CullableGeometry;
	class CullableLight;
	class DebugDrawer;
	class EnvironmentChangedListeners;
	class Cullable;

	class SceneManager
	{
		friend class Root;
	public:
		SceneManager();
		virtual ~SceneManager(void);

		virtual void renderScene() = 0;

		virtual RenderCamera* getMainCamera() = 0;
		virtual RenderContext& getRenderContext() = 0;


		virtual RenderBin& getRenderBin() = 0;

		virtual CullableGeometry* createCullableGeometry() = 0;
		virtual CullableLight* createLight() = 0;

		virtual void addDebugDrawer(DebugDrawer* drawer) = 0;
		virtual void removeDebugDrawer(DebugDrawer* drawer) = 0;

		virtual void addEnvironmentListener(EnvironmentChangedListeners* l) = 0;
		virtual void removeEnvironmentListener(EnvironmentChangedListeners* l) = 0;

		virtual RenderCamera* createCamera() = 0;
		virtual void destroyCamera(RenderCamera* camera) = 0;

		virtual void calculateVisibleObjectsForCamera(RenderCamera* cam, std::vector<Cullable*>& out) = 0;
		virtual void renderCamera(RenderCamera* cam) = 0;

	protected:
		//called from root
		virtual void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) = 0;
		virtual void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) = 0;
		virtual void update(double fTime, float fElapsedTime) = 0;
		virtual void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime) = 0;

		void setEnvironmentTime(float time);
		void setEnvironmentDeltaTime(float dt);
		void setEnvironmentScreenDimension(float w, float h);

	};

	
};
#endif