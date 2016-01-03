#include "Camera.h"
#include "Frustum.h"

namespace Dx11Sandbox
{
    Camera::Camera(void)
        :m_up(0,1,0),
        m_translation(0,0,0),
		m_clipPlane(0, 0, 0, 0),
		m_fovy(0),
        m_aspectRatio(0),
        m_near(0),
        m_far(0),
		m_orthoSize(0),
		m_projectionType(PERSPECTIVE),
        m_viewCacheValid(false),
		m_projectionCacheValid(false),
        m_reflected(false)
        
    {
       

        quatMakeIdentity(m_orientation);
        matMakeIdentity(m_viewMatrix);
		matMakeIdentity(m_projMatrix);
		matMakeIdentity(m_reflMatrix);
        
    }


    Camera::~Camera(void)
    {
    }


	void Camera::copyCameraViewAndProjectionParameters(const Camera& other)
	{
		m_up = other.m_up;
		setTranslation(other.m_translation);
		setOrientation(other.m_orientation);
		m_fovy = other.m_fovy;
		m_aspectRatio = other.m_aspectRatio;
		m_orthoSize = other.m_orthoSize;
		m_near = other.m_near;
		m_far = other.m_far;
		m_projectionCacheValid = false;
		m_viewCacheValid = false;
	}

    const Mat4x4* Camera::getProjectionMatrix()
    {

		if(!m_projectionCacheValid)
		{
			calculateProjection();
		}

        return &m_projMatrix;
    }


	const Mat4x4* Camera::getViewMatrix()
    {
        if(!m_viewCacheValid)
        {
            matMakeIdentity(m_viewMatrix);
			createTransformationTO(m_translation, m_orientation, m_viewMatrix);
            if(m_reflected)
            {
                m_viewMatrix =  m_reflMatrix * m_viewMatrix;

            }

            m_viewCacheValid = true;
        }
        return &m_viewMatrix;
    }

	void Camera::calculateProjection()
	{
		if(m_projectionType == PERSPECTIVE)
		{
			createPerspectiveProjLH(m_fovy, m_aspectRatio, m_near, m_far, m_projMatrix);
		} else {
			createOrthoProjLH(m_orthoSize, m_orthoSize * (1.f / m_aspectRatio), m_near, m_far, m_projMatrix);
		}
		
		m_projectionCacheValid = true;
	}

    void Camera::setProjectionPerspective(FLOAT fovY, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist)
    {
        m_fovy = fovY;
        m_aspectRatio = aspectRatio;
        m_near = nearDist;
        m_far = farDist;
		m_projectionCacheValid = false;
		m_projectionType = PERSPECTIVE;
    }

	void Camera::setProjectionOrthographic(FLOAT size, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist)
	{
		m_orthoSize = size;
        m_aspectRatio = aspectRatio;
        m_near = nearDist;
        m_far = farDist;
		m_projectionCacheValid = false;
		m_projectionType = ORTHOGRAPHIC;
	}

	void Camera::setOrthographicSize(FLOAT size)
	{

		m_projectionCacheValid = false;
		m_orthoSize = size;
	}

	void Camera::setProjectionType(ProjectionType type)
	{
		if(type != m_projectionType)
		{
			m_projectionCacheValid = false;
		}

		m_projectionType = type;

	}

    void Camera::setFOVY( FLOAT y )
    {
        m_fovy = y;
        m_projectionCacheValid = false;
    }
    void Camera::setAspectRatio( FLOAT ar )
    {
        m_aspectRatio = ar;
        m_projectionCacheValid = false;

    }
    void Camera::setNearPlane( FLOAT nearPlane )
    {
        m_near = nearPlane;
        m_projectionCacheValid = false;
    }
    void Camera::setFarPlane( FLOAT farPlane )
    {
        m_far = farPlane;
        m_projectionCacheValid = false;
    }

    FLOAT Camera::getFOVY() const
    {
        return m_fovy;
    }

    FLOAT Camera::getAspectRatio() const
    {
        return m_aspectRatio;
    }
    FLOAT Camera::getNearPlane() const
    {
        return m_near;
    }
    FLOAT Camera::getFarPlance() const
    {
        return m_far;
    }

	FLOAT Camera::getOrthographicSize() const
	{
		return m_orthoSize;
	}
	
	const Vec4& Camera::getClipPlane() const
	{
		return m_clipPlane;
	}

	Camera::ProjectionType Camera::getProjectionType() const
	{
		return m_projectionType;
	}

	void Camera::setClipPlane(const Vec4& plane)
	{
		m_clipPlane = plane;
	}

	


    void  Camera::setTranslation(FLOAT x, FLOAT y, FLOAT z)
    {
        m_viewCacheValid = false;
        m_translation.x = -x;
        m_translation.y = -y;
        m_translation.z = -z;

    }
    void  Camera::addTranslation(FLOAT x, FLOAT y, FLOAT z)
    {
        m_viewCacheValid = false;
        m_translation.x += -x;
        m_translation.y += -y;
        m_translation.z += -z;
    }

    const Vec3&  Camera::getTranslation() const
    {
        return m_translation;
    }

    const Quat& Camera::getOrientation() const
    {
        return m_orientation;
    }

	void Camera::setTranslation(const Vec3& translation)
	{
		m_viewCacheValid = false;
		m_translation = translation;
	}

	void Camera::addTranslation(const Vec3& translation)
	{
		m_viewCacheValid = false;
		m_translation += translation;
	}

	void Camera::setOrientation(const Quat& orientation)
	{
		m_viewCacheValid = false;
		m_orientation = orientation;
	}

    void  Camera::setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_viewCacheValid = false;
        angle = -angle*0.5f;
		m_orientation = createQuat(sin(angle)*x, sin(angle)*y, sin(angle)*z, cos(angle));
    }

    void  Camera::addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_viewCacheValid = false;
        angle = -angle*0.5f;
		m_orientation = m_orientation * createQuat(sin(angle)*x, sin(angle)*y, sin(angle)*z, cos(angle));
    }


	void Camera::lookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
    {
		Vec3 axisUp = up;
		Vec3 axisTo = (at - eye);
        //check if up and  at - eye is perpendicular. If not, make them.
        if( vecDotProduct(axisUp,axisTo) != 0)
        {
            Vec3 temp;
			vecCrossProduct(axisUp, axisTo, temp);
			vecCrossProduct(axisTo, temp, axisUp);
        }
        vecNormalize(axisUp, axisUp);
		vecNormalize(axisTo, axisTo);

        matMakeIdentity(m_viewMatrix);
		createLookAtLH(eye, at, axisUp, m_viewMatrix);

        //calculate transforms
        m_translation = -eye;
        Vec3 ref = Vec3(0.f,0.f,1.f);
        Vec3 rotAxis;
		vecCrossProduct(axisTo, ref, rotAxis);
		vecNormalize(rotAxis, rotAxis);
        float angle;
        

        angle = acos(vecDotProduct( ref, axisTo ))*0.5f;

        m_orientation = createQuat(sin(angle)*rotAxis.x, sin(angle)*rotAxis.y, sin(angle)*rotAxis.z, cos(angle));

        //check if up vector is still "up"
		Vec3 rotatedUp;
		rotateVecByQuat(axisUp, m_orientation, rotatedUp);
        if(vecDotProduct(rotatedUp,up)<0)
        {
            m_orientation = createQuat(axisTo.x*sin(MathUtil::PI*0.5f),axisTo.y*sin(MathUtil::PI*0.5f), axisTo.z*sin(MathUtil::PI*0.5f), cos(MathUtil::PI*0.5f)) * m_orientation;
        }


        m_viewCacheValid = true;
    }

	
    void Camera::moveCameraViewRelative(FLOAT x, FLOAT y, FLOAT z)
    {
        Vec3 dir(0,0,1);
        Vec3 up = m_up;
        Vec3 right;

		Quat conj;
		quatConjugate(m_orientation, conj);
        
		rotateVecByQuat(dir, conj, dir);
		rotateVecByQuat(up, conj, up);
		vecCrossProduct(up, dir, right);

        m_translation -= right*x + up*y + dir*z;
        m_viewCacheValid = false;
    }

    void Camera::rotateCameraViewRelative(FLOAT x, FLOAT y, FLOAT z)
    {
        Vec3 dir(0,0,1);
        Vec3 up(0,1,0);
        Vec3 right;


        

		vecCrossProduct(up, dir, right);

        Quat xrot = createQuat(right.x*sin(x*0.5f),right.y*sin(x*0.5f), right.z*sin(x*0.5f), cos(x*0.5f));
		Quat yrot = createQuat(up.x*sin(y*0.5f), up.y*sin(y*0.5f), up.z*sin(y*0.5f), cos(y*0.5f));
		Quat zrot = createQuat(dir.x*sin(z*0.5f), dir.y*sin(z*0.5f), dir.z*sin(z*0.5f), cos(z*0.5f));

        m_orientation =  yrot * m_orientation * xrot * zrot;
        m_viewCacheValid = false;
    }


    void Camera::setReflectionPlane(Vec3& normal, float d)
    {
        Vec4 plane(normal.x, normal.y, normal.z, d);
		createReflection( plane, m_reflMatrix);

        
    }
    void Camera::setReflectionEnabled(bool val)
    {
        if(val!= m_reflected)
        {
            m_viewCacheValid = false;
        }
        m_reflected = val;
    }

    void Camera::calculateFrustrum(Frustum* frustrum)
    {
        Mat4x4 viewProj((*getViewMatrix()) * (*getProjectionMatrix()));
        
		Frustum::calculateFrustrumFromMatrix(viewProj, *frustrum);
    }
}