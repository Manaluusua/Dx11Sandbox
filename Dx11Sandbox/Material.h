#ifndef DX11SANDBOX_MATERIAL_H
#define DX11SANDBOX_MATERIAL_H


#include "CommonUtilities.h"
#include "DXUT.h"
#include "SDKmisc.h"
#include "RCObject.h"
#include "ReleasePtr.h"
#include "ILDescriptions.h"
#include <map>
#include <d3dx11effect.h>

namespace Dx11Sandbox
{
    class RenderContext;
    class Material : public RCObject
    {
    public:

        friend class MaterialManager;
        virtual ~Material();

        virtual bool loadAndInitializeMaterial(const string& effectName, ID3D11Device* pd3dDevice,
            Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);

        void setTexture(const string shaderVariable, const string textureName);
        ID3DX11Effect* getEffect(){return m_effect;}
        ID3D11InputLayout* getInputLayout(){return m_layout;}

        bool bind(RenderContext* context);

        std::map<string, string>& getTextureReferences(){return m_textureRefs;}

    private:
        Material();
        DISABLE_COPY(Material)
        HRESULT CompileShaderFromFile( WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );


        //key: name of the samplervariable, value: name of the referenced tex to be bind in to the samplervariable
        std::map<string, string> m_textureRefs;

        ReleasePtr<ID3DX11Effect> m_effect;
        ReleasePtr<ID3D11InputLayout> m_layout;
    };

}
#endif