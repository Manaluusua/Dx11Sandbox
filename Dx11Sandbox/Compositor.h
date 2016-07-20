#ifndef DX11SANDBOX_DYNAMICPOOLALLOCATOR_H
#define DX11SANDBOX_DYNAMICPOOLALLOCATOR_H

#include <map>
#include <vector>
#include "CommonUtilities.h"

struct ID3D11Device;


namespace Dx11Sandbox
{
    class Texture;
    class Compositor;
    class Effect;

    class CompositorEffect
    {
        virtual void initialize(ID3D11Device* device, Compositor* comp)=0;
        virtual void applyEffect(ID3D11Device* device, Compositor* comp, Texture* input, Texture* output)=0;
        //if returns false, effect will be removed from the compositor chain.
        virtual bool update(float time, float dt)=0;
        virtual void uninitialize(ID3D11Device* device, Compositor* comp)=0;
    };


    class Compositor
    {
    public:
        Compositor();

        void initialize(ID3D11Device* device, int texWidth, int texHeight);
        void renderFullscreenQuad(Effect* effect);

        void addCompositorEffect(CompositorEffect* effect, string& name);
        void removeCompositorEffect(string& name);

        void addEffectToChain(string& name,int index=-1);
        void removeEffectFromChain(string& name);

        void update(float time, float dt);

        ~Compositor();

         

    private:
        bool m_initialized;

        Texture* m_textures[2];

        std::map<string, CompositorEffect*> m_loadedEffects;
        std::vector<CompositorEffect*> m_effectChain;
    };



}
#endif