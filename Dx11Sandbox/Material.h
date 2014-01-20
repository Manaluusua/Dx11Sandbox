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
       

		Material();
		Material(const Material& other);
		Material& operator=(const Material&);
		virtual ~Material();

        virtual bool loadAndInitializeMaterial(const string& effectName, ID3D11Device* pd3dDevice,
            Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type =  Dx11Sandbox::MeshInputLayouts::POS3NORM3TEX2);

        void setTexture(const string& shaderVariable, ResourceID textureName);
        ID3DX11Effect* getEffect(){return m_effect;}
        ID3D11InputLayout* getInputLayout(){return m_layout;}

        bool bind(RenderContext* context);

        std::map<string, ResourceID>& getTextureReferences(){return m_textureRefs;}

    private:

        HRESULT CompileShaderFromFile( WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );


        //key: name of the samplervariable, value: id of the referenced tex to be bind in to the samplervariable
        std::map<string, ResourceID> m_textureRefs;

        ReleasePtr<ID3DX11Effect> m_effect;
        ReleasePtr<ID3D11InputLayout> m_layout;
    };

}
#endif