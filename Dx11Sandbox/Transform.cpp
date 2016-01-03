#include "Transform.h"
namespace Dx11Sandbox
{

	Transform::Transform(void)
		:m_scale(1.f,1.f,1.f),
		m_translation(0.f, 0.f, 0.f),
		m_cacheValid(false)
	{
		quatMakeIdentity(m_orientation);
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

	void Transform::setScale(const Vec3& scale)
	{
		if(m_scale == scale) return;
		m_cacheValid = false;
		m_scale = scale;
	}
	const Vec3& Transform::getScale() const
	{
		return m_scale;
	}

	void Transform::setTranslation(const Vec3& transl)
	{
		if (m_translation == transl) return;
		m_cacheValid = false;
		m_translation = transl;
	}
	const Vec3& Transform::getTranslation() const
	{
		return m_translation;
	}

	void Transform::setOrientation(const Quat& orient)
	{
		if(m_orientation == orient) return;
		m_cacheValid = false;
		m_orientation = orient;
	}
	const Quat& Transform::getOrientation() const
	{
		return m_orientation;
	}

	const Matrix& Transform::asMatrix() const
	{
		if(m_cacheValid) return m_cachedMatrix;

		createTransformationOST(m_orientation, m_scale, m_translation, m_cachedMatrix);
		m_cacheValid = true;
		return m_cachedMatrix;
	}
}