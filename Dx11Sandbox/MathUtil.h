#ifndef DX11SANDBOX_MATHUTIL_H
#define DX11SANDBOX_MATHUTIL_H
#include <d3dx9math.h>

namespace Dx11Sandbox
{
    namespace MathUtil
    {
        const float PI = 3.14159265;
        D3DXVECTOR3 rotateVec3ByQuat(const D3DXVECTOR3 *vec, const D3DXQUATERNION *quat);

        void calculateOrthogonalVector(const D3DXVECTOR3& vec, D3DXVECTOR3& orthogonal);

		void calculateTangentAndBitangent(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3,
			const D3DXVECTOR2 &t1, const D3DXVECTOR2 &t2, const D3DXVECTOR2 &t3, D3DXVECTOR3 &outTangent, D3DXVECTOR3 &outBitangent);

		void orthogonalizeAndNormalizeTangent(const D3DXVECTOR3 &tangent, const D3DXVECTOR3& normal, D3DXVECTOR3& tangentOut);

		float calculateHandedness(const D3DXVECTOR3 &tangent, const D3DXVECTOR3& bitangent, const D3DXVECTOR3& normal);

		void calculateTangents(long vertexCount, const D3DXVECTOR3 *positions, const D3DXVECTOR3 *normals,
			const D3DXVECTOR2 *texCoords, long trianglesCount, const UINT *indices, D3DXVECTOR4 *outTangents);

		

		float randomFloat();
    }
}

#endif