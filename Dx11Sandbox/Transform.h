#ifndef DX11SANDBOX_TRANSFORM_H
#define DX11SANDBOX_TRANSFORM_H

#include "CommonMath.h"
namespace Dx11Sandbox
{
	class Transform
	{
	public:
		Transform(void);
		~Transform(void);

		Transform(const Transform& other);

		Transform& operator=(const Transform& other);

		void setScale(const Vec3& scale);
		const Vec3& getScale() const;

		void setTranslation(const Vec3& transl);
		const Vec3& getTranslation() const;

		void setOrientation(const Quat& orient);
		const Quat& getOrientation() const;

		const Matrix& asMatrix() const;

	private:
		Vec3 m_scale;
		Vec3 m_translation;
		Quat m_orientation;

		mutable Matrix m_cachedMatrix;
		mutable bool m_cacheValid;

	};
};

#endif
