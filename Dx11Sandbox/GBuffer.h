#ifndef DX11SANDBOX_GBUFFER_H
#define DX11SANDBOX_GBUFFER_H

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

		

	private:

		void allocate();
		void deallocate();

		const unsigned int GBUFFER_TEXTURE_COUNT = 4;
		
		unsigned int m_bufferWidth;
		unsigned int m_bufferHeight;
		RenderContext* m_context;
		Texture** m_textures;
		bool m_isAllocated;
		
		
		
	};
}
#endif
