#include "Camera.h"

namespace Dx11Sandbox
{
    Camera::Camera(void)
        :m_fovy(0),
        m_aspectRatio(0),
        m_near(0),
        m_far(0),
        m_translation(0,0,0),
        m_cacheValid(false)
    {
       

        D3DXQuaternionIdentity(&m_orientation);
        D3DXMatrixIdentity(&m_viewMatrix);
        D3DXMatrixIdentity(&m_projMatrix);
    }


    Camera::~Camera(void)
    {
    }

    const D3DXMATRIX* Camera::getProjectionMatrix()
    {
        return &m_projMatrix;
    }


    const D3DXMATRIX* Camera::getViewMatrix()
    {
        if(!m_cacheValid)
        {
            D3DXMatrixIdentity(&m_viewMatrix);
            D3DXMatrixTranslation(&m_viewMatrix,m_translation.x, m_translation.y, m_translation.z);
            D3DXMatrixRotationQuaternion(&m_viewMatrix, &m_orientation);
        }
        return &m_viewMatrix;
    }

    void Camera::setProjection(FLOAT fovY, FLOAT aspectRatio, FLOAT nearDist, FLOAT farDist)
    {
        m_fovy = fovY;
        m_aspectRatio = aspectRatio;
        m_near = nearDist;
        m_far = farDist;

        D3DXMatrixPerspectiveFovLH(&m_projMatrix, m_fovy, m_aspectRatio, m_near, m_far);
    }


    void  Camera::setTranslation(FLOAT x, FLOAT y, FLOAT z)
    {
        m_cacheValid = false;
        m_translation.x = -x;
        m_translation.y = -y;
        m_translation.z = -z;

    }
    void  Camera::addTranslation(FLOAT x, FLOAT y, FLOAT z)
    {
        m_cacheValid = false;
        m_translation.x += -x;
        m_translation.y += -y;
        m_translation.z += -z;
    }

    const D3DXVECTOR3*  Camera::getTranslation()
    {
        return &m_translation;
    }

    const D3DXQUATERNION* Camera::getOrientation()
    {
        return &m_orientation;
    }

    void  Camera::setOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_cacheValid = false;
        angle = -angle*0.5f;
        m_orientation = D3DXQUATERNION(sin(angle)*x, sin(angle)*y, sin(angle)*z, cos(angle));
    }

    void  Camera::addOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT angle)
    {
        m_cacheValid = false;
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
        float angle;
        

        angle = -D3DXVec3Dot( &ref, &axisTo )*0.5;

        m_orientation = D3DXQUATERNION(sin(angle)*axisUp.x, sin(angle)*axisUp.y, sin(angle)*axisUp.z, cos(angle));


        m_cacheValid = true;
    }

}