#ifndef DX11SANDBOX_RENDERSTATE_H
#define DX11SANDBOX_RENDERSTATE_H



/*struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
*/
#include <D3D11.h>
#include <d3dx9math.h>
#include <vector>

namespace Dx11Sandbox
{
    class Mesh;
    class Material;
    class RenderContext
    {
    public:

        RenderContext(void);
        ~RenderContext(void);


        ID3D11Device* getDevice(){return m_device;}
        void setDevice(ID3D11Device* device){m_device = device;}

        ID3D11DeviceContext* getImmediateContext(){return m_imContext;}
        void setImmediateContext(ID3D11DeviceContext* imContext){m_imContext = imContext;}

        Mesh* getBoundMesh(){return m_boundMesh;}
        Material* getBoundMaterial(){return m_boundMaterial;}

        void bindMesh(Mesh* mesh);
        void bindMaterial(Material* mat);


        void setCustomClipPlane(D3DXVECTOR4& plane){m_customClipPlane = plane;}
        const D3DXVECTOR4& getCustomClipPlane(){return m_customClipPlane;}

        void pushRenderTargets(UINT num, ID3D11RenderTargetView *const *renderTargetViews, ID3D11DepthStencilView *depthStencilView);
		void popRenderTargets();
        

        void clearState();
        
        UINT32 generateUniqueRenderPassID();
        void setCurrentRenderPassID(UINT32 id);
        UINT32 getCurrentRenderPassID();
    private:

		struct RenderTargetsState
		{
			UINT numberOfBoundTargets;
			ID3D11RenderTargetView* renderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView* depthStencil;
		};

		void bindCurrentState();
        D3DXVECTOR4 m_customClipPlane;

		std::vector<RenderTargetsState> m_boundStates;

        Mesh* m_boundMesh;
        Material *m_boundMaterial;

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_imContext;
       

    };


}
#endif