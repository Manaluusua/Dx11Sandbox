#ifndef DX11SANDBOX_MATERIAL_H
#define DX11SANDBOX_MATERIAL_H


#include "CommonUtilities.h"
#include "RCObject.h"
#include "ReleasePtr.h"
#include "RCObjectPtr.h"
#include "D3Dcommon.h"
#include <map>

struct ID3D11Device;
struct ID3D11InputLayout;
namespace Dx11Sandbox
{
    class RenderContext;
	class Shader;
	class InputLayoutDescription;
    class Material : public RCObject
    {
    public:

        friend class MaterialManager;
       

		Material();
		Material(const Material& other);
		Material& operator=(const Material&);
		virtual ~Material();

		virtual bool loadAndInitializeMaterial(const string& effectName, ID3D11Device* pd3dDevice, const InputLayoutDescription& inputDescription);

        void setTexture(const string& shaderVariable, ResourceID textureName);
		Shader* getShader();
		ID3D11InputLayout* getInputLayout();

        bool bind(RenderContext* context);

		std::map<string, ResourceID>& getTextureReferences();

    private:

        HRESULT CompileShaderFromFile( WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );


        //key: name of the samplervariable, value: id of the referenced tex to be bind in to the samplervariable
        std::map<string, ResourceID> m_textureRefs;

        RCObjectPtr<Shader> m_shader;
        ReleasePtr<ID3D11InputLayout> m_layout;
    };

}
#endif