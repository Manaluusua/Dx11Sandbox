#include "Frustum.h"
namespace Dx11Sandbox
{

    Frustum::Frustum()
    {
    }


    Frustum::~Frustum()
    {
    }

	void Frustum::calculateFrustrumFromMatrix(const Matrix &matrix, Frustum& outfrust)
	{
		

		outfrust.rightPlane[0] = matrix[0][3] - matrix[0][0];
		outfrust.rightPlane[1] = matrix[1][3] - matrix[1][0];
		outfrust.rightPlane[2] = matrix[2][3] - matrix[2][0];
		outfrust.rightPlane[3] = matrix[3][3] - matrix[3][0];

		outfrust.leftPlane[0] = matrix[0][3] + matrix[0][0];
		outfrust.leftPlane[1] = matrix[1][3] + matrix[1][0];
		outfrust.leftPlane[2] = matrix[2][3] + matrix[2][0];
		outfrust.leftPlane[3] = matrix[3][3] + matrix[3][0];

		outfrust.topPlane[0] = matrix[0][3] - matrix[0][1];
		outfrust.topPlane[1] = matrix[1][3] - matrix[1][1];
		outfrust.topPlane[2] = matrix[2][3] - matrix[2][1];
		outfrust.topPlane[3] = matrix[3][3] - matrix[3][1];

		outfrust.bottomPlane[0] = matrix[0][3] + matrix[0][1];
		outfrust.bottomPlane[1] = matrix[1][3] + matrix[1][1];
		outfrust.bottomPlane[2] = matrix[2][3] + matrix[2][1];
		outfrust.bottomPlane[3] = matrix[3][3] + matrix[3][1];

		outfrust.nearPlane[0] = matrix[0][2];
		outfrust.nearPlane[1] = matrix[1][2];
		outfrust.nearPlane[2] = matrix[2][2];
		outfrust.nearPlane[3] = matrix[3][2];

		outfrust.farPlane[0] = matrix[0][3] - matrix[0][2];
		outfrust.farPlane[1] = matrix[1][3] - matrix[1][2];
		outfrust.farPlane[2] = matrix[2][3] - matrix[2][2];
		outfrust.farPlane[3] = matrix[3][3] - matrix[3][2];

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

		line[0] = leftPlane[0];
		line[1] = rightPlane[0];
		line[2] = topPlane[0];
		line[3] = bottomPlane[0];

		out.x1x2x3x4 = _mm_load_ps(line);

		line[0] = leftPlane[1];
		line[1] = rightPlane[1];
		line[2] = topPlane[1];
		line[3] = bottomPlane[1];

		out.y1y2y3y4 = _mm_load_ps(line);

		line[0] = leftPlane[2];
		line[1] = rightPlane[2];
		line[2] = topPlane[2];
		line[3] = bottomPlane[2];

		out.z1z2z3z4 = _mm_load_ps(line);

		line[0] = leftPlane[3];
		line[1] = rightPlane[3];
		line[2] = topPlane[3];
		line[3] = bottomPlane[3];

		out.d1d2d3d4 = _mm_load_ps(line);

		line[0] = farPlane[0];
		line[1] = nearPlane[0];
		line[2] = farPlane[0];
		line[3] = nearPlane[0];

		out.x5x6x5x6 = _mm_load_ps(line);

		line[0] = farPlane[1];
		line[1] = nearPlane[1];
		line[2] = farPlane[1];
		line[3] = nearPlane[1];

		out.y5y6y5y6 = _mm_load_ps(line);

		line[0] = farPlane[2];
		line[1] = nearPlane[2];
		line[2] = farPlane[2];
		line[3] = nearPlane[2];

		out.z5z6z5z6 = _mm_load_ps(line);

		line[0] = farPlane[3];
		line[1] = nearPlane[3];
		line[2] = farPlane[3];
		line[3] = nearPlane[3];

		out.d5d6d5d6 = _mm_load_ps(line);

	}
    
}