#ifndef DX11SANDBOX_CAMERA_H
#define DX11SANDBOX_CAMERA_H
#include "MathUtil.h"
#include "RCObject.h"
#include <D3D11.h>


namespace Dx11Sandbox
{
	class Frustum;
    class Camera 
    {
    public:

		enum ProjectionType
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};

        Camera(void);
        virtual ~Camera(void);

        void setProjectionPerspective(FLOAT fovY, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist);
		void setProjectionOrthographic(FLOAT size, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist);

        const Mat4x4* getProjectionMatrix();
		const Mat4x4* getViewMatrix();
    
        void setTranslation(FLOAT x, FLOAT y, FLOAT z);
		void setTranslation(const Vec3& translation);
        void addTranslation(FLOAT x, FLOAT y, FLOAT z);
		void addTranslation(const Vec3& translation);
        const Vec3& getTranslation() const;

        const Quat& getOrientation() const;
		void setOrientation(const Quat& orientation);
        void setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);
        void addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle);

        void lookAt(const Vec3& eye, const Vec3& at,const Vec3& up);

        void moveCameraViewRelative(FLOAT x, FLOAT y, FLOAT z);
        void rotateCameraViewRelative(FLOAT x, FLOAT y, FLOAT z);

        void setUp(Vec3& up){m_up = up;}
        const Vec3& getUp() const{return m_up;}

        void setReflectionPlane(Vec3& normal, float d);
        void setReflectionEnabled(bool val);

        void setFOVY( FLOAT y );
        void setAspectRatio( FLOAT ar );
        void setNearPlane( FLOAT nearPlane );
        void setFarPlane( FLOAT farPlane );
		void setOrthographicSize(FLOAT size);
		void setProjectionType(ProjectionType type);

		const Vec4& getClipPlane() const;
		void setClipPlane(const Vec4& plane);

		virtual void copyCameraViewAndProjectionParameters(const Camera& other);

        FLOAT getFOVY() const;
        FLOAT getAspectRatio() const;
        FLOAT getNearPlane() const;
        FLOAT getFarPlance() const;
		FLOAT getOrthographicSize() const;
		
		
		ProjectionType getProjectionType() const;

        void calculateFrustrum(Frustum* frustrum);

    protected:

		void calculateProjection();

        Mat4x4 m_viewMatrix;
		Mat4x4 m_projMatrix;

		Mat4x4 m_reflMatrix;

		Quat m_orientation;
		Vec4 m_clipPlane;
        Vec3 m_up;
        Vec3 m_translation;

		

		FLOAT m_fovy;
        FLOAT m_aspectRatio;
        FLOAT m_near;
        FLOAT m_far;
		FLOAT m_orthoSize;
		
		ProjectionType m_projectionType;

		bool m_viewCacheValid;
		bool m_projectionCacheValid;

        bool m_reflected;
    };
}
#endif