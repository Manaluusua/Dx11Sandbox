#ifndef DX11SANDBOX_MATHUTIL_H
#define DX11SANDBOX_MATHUTIL_H
#include <d3dx9math.h>

namespace Dx11Sandbox
{
    namespace MathUtil
    {
        const double PI = 3.14159265;
        D3DXVECTOR3 rotateVec3ByQuat(const D3DXVECTOR3 *vec, const D3DXQUATERNION *quat);

        void calculateOrthogonalVector(const D3DXVECTOR3& vec, D3DXVECTOR3& orthogonal);

		void calculateTangents(long vertexCount, const D3DXVECTOR3 *positions, const D3DXVECTOR3 *normals,
			const D3DXVECTOR2 *texCoords, long trianglesCount, const UINT *indices, D3DXVECTOR3 *outTangents);

		float randomFloat();
    }
}

#endif