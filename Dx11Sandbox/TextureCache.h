#ifndef DX11SANDBOX_TEXTURECACHE_H
#define DX11SANDBOX_TEXTURECACHE_H

#include "Singleton.h"
#include "CommonUtilities.h"
#include <vector>
namespace Dx11Sandbox
{
	class Texture;
	class TextureCache : public Singleton<TextureCache>
	{
		SINGLETON(TextureCache)
	public:

		enum TextureType
		{
			GALBEDO,
			GNORMAL,
			GSPECULAR,
			GENVIRONMENT,
			SCENEDEPTH,
			SHADOWMAPS,

			FirstInvalidIndex
		};

		virtual ~TextureCache();

		void setCachedTextures(TextureType type, Texture** textures, size_t textureCount);
		void getCachedTextures(TextureType type, Texture** texturesOut, size_t& textureCountOut); //valid only before next setCachedTextures call

	private:

		std::vector<std::vector<Texture*> > m_caches;

		DISABLE_COPY(TextureCache)
		TextureCache();
	};

	inline void TextureCache::setCachedTextures(TextureType type, Texture** textures, size_t textureCount)
	{
		std::vector<Texture*>& texArray = m_caches[type];
		texArray.clear();
		
		for (size_t i = 0; i < textureCount; ++i)
		{
			texArray.push_back(textures[i]);
		}
	}
	inline void TextureCache::getCachedTextures(TextureType type, Texture** texturesOut, size_t& textureCountOut)
	{
		std::vector<Texture*>& texArray = m_caches[type];
		textureCountOut = texArray.size();
		texturesOut = &texArray[0];

	}
}

#endif
