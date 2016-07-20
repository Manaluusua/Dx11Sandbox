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

    const Matrix* Camera::getProjectionMatrix()
    {

		if(!m_projectionCacheValid)
		{
			calculateProjection();
		}

        return &m_projMatrix;
    }


	const Matrix* Camera::getViewMatrix()
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
        m_translation[0] = -x;
        m_translation[1] = -y;
        m_translation[2] = -z;

    }
    void  Camera::addTranslation(FLOAT x, FLOAT y, FLOAT z)
    {
        m_viewCacheValid = false;
        m_translation[0] += -x;
        m_translation[1] += -y;
        m_translation[2] += -z;
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
		m_orientation = createQuatFromAxisAngle(x, y, z, angle);
    }

    void  Camera::addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_viewCacheValid = false;
        angle = -angle*0.5f;
		m_orientation = multiplyQuat(m_orientation, createQuatFromAxisAngle(x, y, z, angle), m_orientation);
    }


	void Camera::lookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
    {
		Vec3 axisUp = up;
		Vec3 axisTo = (at - eye);
        //check if up and  at - eye is perpendicular. If not, make them.
        if( dot(axisUp,axisTo) != 0)
        {
            Vec3 temp;
			cross(temp, axisUp, axisTo);
			cross(axisUp, axisTo, temp);
        }
        normalize(axisUp);
		normalize(axisTo);

        matMakeIdentity(m_viewMatrix);
		createLookAtLH(eye, at, axisUp, m_viewMatrix);

        //calculate transforms
        m_translation = -eye;
        Vec3 ref = Vec3(0.f,0.f,1.f);
        Vec3 rotAxis;
		cross(rotAxis, axisTo, ref);
		normalize(rotAxis);
        float angle;
        

        angle = acos(dot( ref, axisTo ))*0.5f;

		m_orientation = createQuatFromAxisAngle(rotAxis, angle);

        //check if up vector is still "up"
		Vec3 rotatedUp;
		rotateVecByQuat(axisUp, m_orientation, rotatedUp);
        if(dot(rotatedUp,up)<0)
        {
			m_orientation = multiplyQuat(createQuatFromAxisAngle(axisTo, MathUtil::PI), m_orientation, m_orientation);
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
		cross(right, up, dir);

        m_translation -= right*x + up*y + dir*z;
        m_viewCacheValid = false;
    }

    void Camera::rotateCameraViewRelative(FLOAT x, FLOAT y, FLOAT z)
    {
        Vec3 dir(0,0,1);
        Vec3 up(0,1,0);
        Vec3 right;


        

		cross(right,up, dir);

		Quat xrot = createQuatFromAxisAngle(right, x);
		Quat yrot = createQuatFromAxisAngle(up, y);
		Quat zrot = createQuatFromAxisAngle(dir, z);

		m_orientation = multiplyQuat(multiplyQuat(multiplyQuat(yrot, m_orientation, m_orientation), xrot, m_orientation), zrot, m_orientation);
        m_viewCacheValid = false;
    }


    void Camera::setReflectionPlane(Vec3& normal, float d)
    {
        Vec4 plane(normal[0], normal[1], normal[2], d);
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
        Matrix viewProj((*getViewMatrix()) * (*getProjectionMatrix()));
        
		Frustum::calculateFrustrumFromMatrix(viewProj, *frustrum);
    }
}