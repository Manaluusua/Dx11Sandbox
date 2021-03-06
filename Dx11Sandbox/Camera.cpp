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
       

        D3DXQuaternionIdentity(&m_orientation);
        D3DXMatrixIdentity(&m_viewMatrix);
        D3DXMatrixIdentity(&m_projMatrix);
        D3DXMatrixIdentity(&m_reflMatrix);
        
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

    const D3DXMATRIX* Camera::getProjectionMatrix()
    {

		if(!m_projectionCacheValid)
		{
			calculateProjection();
		}

        return &m_projMatrix;
    }


    const D3DXMATRIX* Camera::getViewMatrix()
    {
        if(!m_viewCacheValid)
        {
            D3DXMATRIX rot;
            D3DXMatrixIdentity(&m_viewMatrix);

            D3DXVECTOR4 transl(m_translation,1);
            D3DXMatrixRotationQuaternion(&rot, &m_orientation);

            D3DXMatrixTranslation(&m_viewMatrix,transl.x, transl.y, transl.z);

            m_viewMatrix *= rot;

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
			D3DXMatrixPerspectiveFovLH(&m_projMatrix, m_fovy, m_aspectRatio, m_near, m_far);
		} else {
			D3DXMatrixOrthoLH(&m_projMatrix, m_orthoSize, m_orthoSize * (1.f / m_aspectRatio), m_near, m_far); 
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
	
	const D3DXVECTOR4& Camera::getClipPlane() const
	{
		return m_clipPlane;
	}

	Camera::ProjectionType Camera::getProjectionType() const
	{
		return m_projectionType;
	}

	void Camera::setClipPlane(const D3DXVECTOR4& plane)
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

    const D3DXVECTOR3&  Camera::getTranslation() const
    {
        return m_translation;
    }

    const D3DXQUATERNION& Camera::getOrientation() const
    {
        return m_orientation;
    }

	void Camera::setTranslation(const D3DXVECTOR3& translation)
	{
		m_viewCacheValid = false;
		m_translation = translation;
	}

	void Camera::addTranslation(const D3DXVECTOR3& translation)
	{
		m_viewCacheValid = false;
		m_translation += translation;
	}

	void Camera::setOrientation(const D3DXQUATERNION& orientation)
	{
		m_viewCacheValid = false;
		m_orientation = orientation;
	}

    void  Camera::setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_viewCacheValid = false;
        angle = -angle*0.5f;
        m_orientation = D3DXQUATERNION(sin(angle)*x, sin(angle)*y, sin(angle)*z, cos(angle));
    }

    void  Camera::addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_viewCacheValid = false;
        angle = -angle*0.5f;
        m_orientation *= D3DXQUATERNION(sin(angle)*x, sin(angle)*y, sin(angle)*z, cos(angle));
    }


    void Camera::lookAt(const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up)
    {
        D3DXVECTOR3 axisUp = up;
        D3DXVECTOR3 axisTo = (at-eye);
        //check if up and  at - eye is perpendicular. If not, make them.
        if( D3DXVec3Dot(&axisUp,&axisTo) != 0)
        {
            D3DXVECTOR3 temp;
            D3DXVec3Cross(&temp, &axisUp, &axisTo);
            D3DXVec3Cross(&axisUp, &axisTo, &temp);
        }
        D3DXVec3Normalize(&axisUp, &axisUp);
        D3DXVec3Normalize(&axisTo, &axisTo);

        D3DXMatrixIdentity(&m_viewMatrix);
        D3DXMatrixLookAtLH(&m_viewMatrix,&eye,&at,&axisUp);

        //calculate transforms
        m_translation = -eye;
        D3DXVECTOR3 ref = D3DXVECTOR3(0,0,1);
        D3DXVECTOR3 rotAxis;
        D3DXVec3Cross(&rotAxis,&axisTo,&ref);
        D3DXVec3Normalize(&rotAxis, &rotAxis);
        float angle;
        

        angle = acos(D3DXVec3Dot( &ref, &axisTo ))*0.5f;

        m_orientation = D3DXQUATERNION(sin(angle)*rotAxis.x, sin(angle)*rotAxis.y, sin(angle)*rotAxis.z, cos(angle));

        //check if up vector is still "up"
        
        D3DXVECTOR3 rotatedUp = MathUtil::rotateVec3ByQuat(&axisUp, &m_orientation);
        if(D3DXVec3Dot(&rotatedUp,&up)<0)
        {
            m_orientation = D3DXQUATERNION(axisTo.x*sin(MathUtil::PI*0.5f),axisTo.y*sin(MathUtil::PI*0.5f), axisTo.z*sin(MathUtil::PI*0.5f), cos(MathUtil::PI*0.5f)) * m_orientation;
        }


        m_viewCacheValid = true;
    }

	
    void Camera::moveCameraViewRelative(FLOAT x, FLOAT y, FLOAT z)
    {
        D3DXVECTOR3 dir(0,0,1);
        D3DXVECTOR3 up = m_up;
        D3DXVECTOR3 right;

        D3DXQUATERNION conj;
        D3DXQuaternionConjugate(&conj, &m_orientation);
        
        dir = MathUtil::rotateVec3ByQuat(&dir,&conj);
        up = MathUtil::rotateVec3ByQuat(&up,&conj);
        D3DXVec3Cross(&right, &up, &dir);

        m_translation -= x*right + y*up + z*dir;
        m_viewCacheValid = false;
    }

    void Camera::rotateCameraViewRelative(FLOAT x, FLOAT y, FLOAT z)
    {
        D3DXVECTOR3 dir(0,0,1);
        D3DXVECTOR3 up(0,1,0);
        D3DXVECTOR3 right;


        

        D3DXVec3Cross(&right, &up, &dir);

        D3DXQUATERNION xrot(right.x*sin(x*0.5f),right.y*sin(x*0.5f), right.z*sin(x*0.5f), cos(x*0.5f));
        D3DXQUATERNION yrot(up.x*sin(y*0.5f),up.y*sin(y*0.5f), up.z*sin(y*0.5f), cos(y*0.5f));
        D3DXQUATERNION zrot(dir.x*sin(z*0.5f),dir.y*sin(z*0.5f), dir.z*sin(z*0.5f), cos(z*0.5f));

        m_orientation =  yrot * m_orientation * xrot * zrot;
        m_viewCacheValid = false;
    }


    void Camera::setReflectionPlane(D3DXVECTOR3& normal, float d)
    {
        D3DXPLANE plane(normal.x, normal.y, normal.z, d);
        D3DXMatrixReflect(&m_reflMatrix, &plane);

        
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
        D3DXMATRIX viewProj((*getViewMatrix()) * (*getProjectionMatrix()));
        
		Frustum::calculateFrustrumFromMatrix(viewProj, *frustrum);
    }
}