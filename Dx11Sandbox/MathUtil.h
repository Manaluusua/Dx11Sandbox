#ifndef DX11SANDBOX_MATHUTIL_H
#define DX11SANDBOX_MATHUTIL_H
#include "CommonMath.h"

namespace Dx11Sandbox
{
    namespace MathUtil
    {
        const float PI = 3.14159265f;

		

        
        void calculateOrthogonalVector(const Vec3& vec, Vec3& orthogonal);

		void calculateTangentAndBitangent(const Vec3& p1, const Vec3& p2, const Vec3& p3,
			const Vec2 &t1, const Vec2 &t2, const Vec2 &t3, Vec3 &outTangent, Vec3 &outBitangent);

		void orthogonalizeAndNormalizeTangent(const Vec3 &tangent, const Vec3& normal, Vec3& tangentOut);

		float calculateHandedness(const Vec3 &tangent, const Vec3& bitangent, const Vec3& normal);

		void calculateTangents(long vertexCount, const Vec3 *positions, const Vec3 *normals,
			const Vec2 *texCoords, long trianglesCount, const unsigned int *indices, Vec4 *outTangents);

		

		float randomFloat();
    }
}

#endif