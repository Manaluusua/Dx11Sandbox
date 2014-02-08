#include "Transform.h"
namespace Dx11Sandbox
{

	Transform::Transform(void)
		:m_scale(1.f,1.f,1.f),
		m_translation(0.f, 0.f, 0.f),
		m_cacheValid(false)
	{
		D3DXQuaternionIdentity(&m_orientation);
	}


	Transform::~Transform(void)
	{
	}

	Transform::Transform(const Transform& other)
		:m_scale(other.m_scale),
		m_translation(other.m_translation),
		m_orientation(other.m_orientation),
		m_cachedMatrix(other.m_cachedMatrix),
		m_cacheValid(other.m_cacheValid)
	{
	}

	Transform& Transform::operator=(const Transform& other)
	{
		if(this == &other) return *this;

		m_scale = other.m_scale;
		m_translation = other.m_translation;
		m_orientation = other.m_orientation;
		m_cacheValid = other.m_cacheValid;
		return *this;
	}

	void Transform::setScale(const D3DXVECTOR3& scale)
	{
		if(m_scale == scale) return;
		m_cacheValid = false;
		m_scale = scale;
	}
	const D3DXVECTOR3& Transform::getScale() const
	{
		return m_scale;
	}

	void Transform::setTranslation(const D3DXVECTOR3& transl)
	{
		if(m_translation == transl) return;
		m_cacheValid = false;
		m_translation = transl;
	}
	const D3DXVECTOR3& Transform::getTranslation() const
	{
		return m_translation;
	}

	void Transform::setOrientation(const D3DXQUATERNION& orient)
	{
		if(m_orientation == orient) return;
		m_cacheValid = false;
		m_orientation = orient;
	}
	const D3DXQUATERNION& Transform::getOrientation() const
	{
		return m_orientation;
	}

	const D3DXMATRIX& Transform::asMatrix() const
	{
		if(m_cacheValid) return m_cachedMatrix;

		//calculate matrix
		D3DXMATRIX rot;
		
		D3DXMatrixIdentity(&m_cachedMatrix);

		D3DXMatrixScaling(&m_cachedMatrix, m_scale.x, m_scale.y, m_scale.z);
        D3DXMatrixRotationQuaternion(&rot, &m_orientation);

        m_cachedMatrix = m_cachedMatrix * rot;
		m_cachedMatrix(3,0) = m_translation.x;
		m_cachedMatrix(3,1) = m_translation.y;
		m_cachedMatrix(3,2) = m_translation.z;


		return m_cachedMatrix;
		m_cacheValid = true;
	}
}