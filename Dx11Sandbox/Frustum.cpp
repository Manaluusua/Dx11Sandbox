#include "Frustum.h"
namespace Dx11Sandbox
{

    Frustum::Frustum()
    {
    }


    Frustum::~Frustum()
    {
    }

	void Frustum::calculateFrustrumFromMatrix(const Matrix &mat, Frustum& outfrust)
	{
		Matrix4x4 matrix;
		matrixToMatrix4x4(mat, matrix);


		outfrust.rightPlane.x = matrix._14 - matrix._11;
		outfrust.rightPlane.y = matrix._24 - matrix._21;
		outfrust.rightPlane.z = matrix._34 - matrix._31;
		outfrust.rightPlane.w = matrix._44 - matrix._41;

		outfrust.leftPlane.x = matrix._14 + matrix._11;
		outfrust.leftPlane.y = matrix._24 + matrix._21;
		outfrust.leftPlane.z = matrix._34 + matrix._31;
		outfrust.leftPlane.w = matrix._44 + matrix._41;

		outfrust.topPlane.x = matrix._14 - matrix._12;
		outfrust.topPlane.y = matrix._24 - matrix._22;
		outfrust.topPlane.z = matrix._34 - matrix._32;
		outfrust.topPlane.w = matrix._44 - matrix._42;

		outfrust.bottomPlane.x = matrix._14 + matrix._12;
		outfrust.bottomPlane.y = matrix._24 + matrix._22;
		outfrust.bottomPlane.z = matrix._34 + matrix._32;
		outfrust.bottomPlane.w = matrix._44 + matrix._42;

		outfrust.nearPlane.x = matrix._13;
		outfrust.nearPlane.y = matrix._23;
		outfrust.nearPlane.z = matrix._33;
		outfrust.nearPlane.w = matrix._43;

		outfrust.farPlane.x = matrix._14 - matrix._13;
		outfrust.farPlane.y = matrix._24 - matrix._23;
		outfrust.farPlane.z = matrix._34 - matrix._33;
		outfrust.farPlane.w = matrix._44 - matrix._43;

		planeNormalize(outfrust.leftPlane, outfrust.leftPlane);
		planeNormalize(outfrust.rightPlane, outfrust.rightPlane);
		planeNormalize(outfrust.topPlane, outfrust.topPlane);
		planeNormalize(outfrust.bottomPlane, outfrust.bottomPlane);
		planeNormalize(outfrust.nearPlane, outfrust.nearPlane);
		planeNormalize(outfrust.farPlane, outfrust.farPlane);

	}
	void Frustum::calculateMatrixFromFrustrum(const Frustum& frust, Matrix &matrix)
	{

	}
	

	void Frustum::convertToSimdFrustrum(Frustum::SIMDFrustrum& out) const
	{

		__declspec(align(16)) float line[4];

		line[0] = leftPlane.x;
		line[1] = rightPlane.x;
		line[2] = topPlane.x;
		line[3] = bottomPlane.x;

		out.x1x2x3x4 = _mm_load_ps(line);

		line[0] = leftPlane.y;
		line[1] = rightPlane.y;
		line[2] = topPlane.y;
		line[3] = bottomPlane.y;

		out.y1y2y3y4 = _mm_load_ps(line);

		line[0] = leftPlane.z;
		line[1] = rightPlane.z;
		line[2] = topPlane.z;
		line[3] = bottomPlane.z;

		out.z1z2z3z4 = _mm_load_ps(line);

		line[0] = leftPlane.w;
		line[1] = rightPlane.w;
		line[2] = topPlane.w;
		line[3] = bottomPlane.w;

		out.d1d2d3d4 = _mm_load_ps(line);

		line[0] = farPlane.x;
		line[1] = nearPlane.x;
		line[2] = farPlane.x;
		line[3] = nearPlane.x;

		out.x5x6x5x6 = _mm_load_ps(line);

		line[0] = farPlane.y;
		line[1] = nearPlane.y;
		line[2] = farPlane.y;
		line[3] = nearPlane.y;

		out.y5y6y5y6 = _mm_load_ps(line);

		line[0] = farPlane.z;
		line[1] = nearPlane.z;
		line[2] = farPlane.z;
		line[3] = nearPlane.z;

		out.z5z6z5z6 = _mm_load_ps(line);

		line[0] = farPlane.w;
		line[1] = nearPlane.w;
		line[2] = farPlane.w;
		line[3] = nearPlane.w;

		out.d5d6d5d6 = _mm_load_ps(line);

	}
    
}