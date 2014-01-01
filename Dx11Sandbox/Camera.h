#ifndef DX11SANDBOX_CAMERA_H
#define DX11SANDBOX_CAMERA_H
#include <D3D11.h>
#include "MathUtil.h"
#include "Frustrum.h"
#include "RCObject.h"

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
		void setTranslation(const D3DXVECTOR3& translation);
        void addTranslation(FLOAT x, FLOAT y, FLOAT z);
		void addTranslation(const D3DXVECTOR3& translation);
        const D3DXVECTOR3& getTranslation() const;

        const D3DXQUATERNION& getOrientation() const;
		void setOrientation(const D3DXQUATERNION& orientation);
        void setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);
        void addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);

        void lookAt(const D3DXVECTOR3& eye, const D3DXVECTOR3& at,const D3DXVECTOR3& up);

        void moveCameraViewRelative(FLOAT x, FLOAT y, FLOAT z);
        void rotateCameraViewRelative(FLOAT x, FLOAT y, FLOAT z);

        void setUp(D3DXVECTOR3& up){m_up = up;}
        const D3DXVECTOR3& getUp() const{return m_up;}

        void setReflectionPlane(D3DXVECTOR3& normal, float d);
        void setReflectionEnabled(bool val);

        void setFOVY( FLOAT y );
        void setAspectRatio( FLOAT ar );
        void setNearPlane( FLOAT nearPlane );
        void setFarPlane( FLOAT farPlane );

		
		const D3DXVECTOR4& getClipPlane() const;
		void setClipPlane(const D3DXVECTOR4& plane);

		virtual void copyCameraViewAndProjectionParameters(const Camera& other);

        FLOAT getFOVY() const;
        FLOAT getAspectRatio() const;
        FLOAT getNearPlane() const;
        FLOAT getFarPlance() const;

		

        void calculateFrustrum(Dx11Sandbox::Frustrum* frustrum);

    protected:

		void calculateProjection();

        D3DXMATRIX m_viewMatrix;
        D3DXMATRIX m_projMatrix;

        D3DXMATRIX m_reflMatrix;

		D3DXQUATERNION m_orientation;

        D3DXVECTOR3 m_up;

        D3DXVECTOR3 m_translation;

		D3DXVECTOR4 m_clipPlane;

		FLOAT m_fovy;
        FLOAT m_aspectRatio;
        FLOAT m_near;
        FLOAT m_far;

		

		bool m_viewCacheValid;
		bool m_projectionCacheValid;

        bool m_reflected;
    };
}
#endif