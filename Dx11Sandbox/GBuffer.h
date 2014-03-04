#ifndef DX11SANDBOX_GBUFFER_H
#define DX11SANDBOX_GBUFFER_H


struct ID3D11RenderTargetView;

namespace Dx11Sandbox
{
	class Texture;
	class RenderContext;
	class GBuffer
	{
	public:
		
		enum GBufferTexture{
			ALBEDO = 0,
			NORMAL = 1,
			SPECULAR = 2,
			DEPTH = 3
		};
		GBuffer(RenderContext* context);
		~GBuffer();
		void allocateBuffer(unsigned int w, unsigned int h);

		void setAsRenderTargets();
		

	private:

		void allocate();
		void deallocate();

		static const unsigned int GBUFFER_TEXTURE_COUNT;
		
		unsigned int m_bufferWidth;
		unsigned int m_bufferHeight;
		RenderContext* m_context;
		Texture** m_textures;
		ID3D11RenderTargetView** m_renderTargets;
		bool m_isAllocated;
		
		
		
	};
}
#endif
