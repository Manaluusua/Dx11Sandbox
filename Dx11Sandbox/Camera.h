#ifndef DX11SANDBOX_CAMERA_H
#define DX11SANDBOX_CAMERA_H
#include <D3D11.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{
    class Camera
    {
    public:
        Camera(void);
        virtual ~Camera(void);

        void setProjection(FLOAT fovY, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist);

        const D3DXMATRIX* getProjectionMatrix();
        const D3DXMATRIX* getViewMatrix();
    
        void setTranslation(FLOAT x, FLOAT y, FLOAT z);
        void addTranslation(FLOAT x, FLOAT y, FLOAT z);
        const D3DXVECTOR3* getTranslation();

        const D3DXQUATERNION* getOrientation();
        void setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);
        void addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);

        void lookAt(const D3DXVECTOR3& eye, const D3DXVECTOR3& at,const D3DXVECTOR3& up);

    private:

        FLOAT m_fovy;
        FLOAT m_aspectRatio;
        FLOAT m_near;
        FLOAT m_far;

        D3DXVECTOR3 m_translation;
        D3DXQUATERNION m_orientation;

        bool m_cacheValid;

        D3DXMATRIX m_viewMatrix;
        D3DXMATRIX m_projMatrix;
    };
}
#endif