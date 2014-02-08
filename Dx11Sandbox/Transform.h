#ifndef DX11SANDBOX_TRANSFORM_H
#define DX11SANDBOX_TRANSFORM_H

#include <d3dx9math.h>
namespace Dx11Sandbox
{
	class Transform
	{
	public:
		Transform(void);
		~Transform(void);

		Transform(const Transform& other);

		Transform& operator=(const Transform& other);

		void setScale(const D3DXVECTOR3& scale);
		const D3DXVECTOR3& getScale() const;

		void setTranslation(const D3DXVECTOR3& transl);
		const D3DXVECTOR3& getTranslation() const;

		void setOrientation(const D3DXQUATERNION& orient);
		const D3DXQUATERNION& getOrientation() const;

		const D3DXMATRIX& asMatrix() const;

	private:
		D3DXVECTOR3 m_scale;
		D3DXVECTOR3 m_translation;
		D3DXQUATERNION m_orientation;

		mutable D3DXMATRIX m_cachedMatrix;
		mutable bool m_cacheValid;

	};
};

#endif
