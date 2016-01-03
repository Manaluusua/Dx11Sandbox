#ifndef DX11SANDBOX_RENDERSTATE_H
#define DX11SANDBOX_RENDERSTATE_H



/*struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
*/
#include "CommonMath.h"
#include <D3D11.h>
#include <vector>

namespace Dx11Sandbox
{
    class Mesh;
    class Material;
	class Texture;
    class RenderContext
    {
    public:

        RenderContext(void);
        ~RenderContext(void);


		ID3D11Device* getDevice();
		void setDevice(ID3D11Device* device);

		ID3D11DeviceContext* getImmediateContext();
		void setImmediateContext(ID3D11DeviceContext* imContext);

		Mesh* getBoundMesh();
		Material* getBoundMaterial();

        void bindMesh(Mesh* mesh);
        void bindMaterial(Material* mat);

		Texture* getDefaultDepthStencilTexture();

		void setCustomClipPlane(Vec4& plane);
		const Vec4& getCustomClipPlane();

		void disableDepthStencil(bool val);
		void pushRenderTargets(uint32_t num, ID3D11RenderTargetView *const *renderTargetViews, ID3D11DepthStencilView *depthStencilView);
		void popRenderTargets();
        
		void setDefaultViewport(D3D11_VIEWPORT* viewport);
		void pushViewports(uint32_t viewportCount, D3D11_VIEWPORT* viewports);
		void popViewports();

        void clearState();
        
    private:

		void createDepthStencil(int w, int h);

		struct RenderTargetsState
		{
			uint32_t numberOfBoundTargets;
			ID3D11RenderTargetView* renderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView* depthStencil;
		};

		struct ViewportState{
			D3D11_VIEWPORT* viewports;
			uint32_t viewportCount;
		};

		void bindCurrentRenderTargetState(bool bindDepthStencil = true);
		void bindCurrentViewports();
        Vec4 m_customClipPlane;

		std::vector<RenderTargetsState> m_boundStates;
		std::vector<ViewportState> m_viewPortStates;
		
		D3D11_VIEWPORT m_defaultViewport;
		
		Mesh* m_boundMesh;
		Material *m_boundMaterial;
		Texture* m_defaultDepthStencil;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_imContext;

    };


}
#endif