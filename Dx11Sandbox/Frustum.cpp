#include "Frustum.h"
namespace Dx11Sandbox
{

    Frustum::Frustum()
    {
    }


    Frustum::~Frustum()
    {
    }

	void Frustum::calculateFrustrumFromMatrix(const D3DXMATRIX &matrix, Frustum& outfrust)
	{
		outfrust.rightPlane.a = matrix._14 - matrix._11;
		outfrust.rightPlane.b = matrix._24 - matrix._21;
		outfrust.rightPlane.c = matrix._34 - matrix._31;
		outfrust.rightPlane.d = matrix._44 - matrix._41;

		outfrust.leftPlane.a = matrix._14 + matrix._11;
		outfrust.leftPlane.b = matrix._24 + matrix._21;
		outfrust.leftPlane.c = matrix._34 + matrix._31;
		outfrust.leftPlane.d = matrix._44 + matrix._41;

		outfrust.topPlane.a = matrix._14 - matrix._12;
		outfrust.topPlane.b = matrix._24 - matrix._22;
		outfrust.topPlane.c = matrix._34 - matrix._32;
		outfrust.topPlane.d = matrix._44 - matrix._42;

		outfrust.bottomPlane.a = matrix._14 + matrix._12;
		outfrust.bottomPlane.b = matrix._24 + matrix._22;
		outfrust.bottomPlane.c = matrix._34 + matrix._32;
		outfrust.bottomPlane.d = matrix._44 + matrix._42;

		outfrust.nearPlane.a = matrix._13;
		outfrust.nearPlane.b = matrix._23;
		outfrust.nearPlane.c = matrix._33;
		outfrust.nearPlane.d = matrix._43;

		outfrust.farPlane.a = matrix._14 - matrix._13;
		outfrust.farPlane.b = matrix._24 - matrix._23;
		outfrust.farPlane.c = matrix._34 - matrix._33;
		outfrust.farPlane.d = matrix._44 - matrix._43;

		D3DXPlaneNormalize(&outfrust.leftPlane, &outfrust.leftPlane);
		D3DXPlaneNormalize(&outfrust.rightPlane, &outfrust.rightPlane);
		D3DXPlaneNormalize(&outfrust.topPlane, &outfrust.topPlane);
		D3DXPlaneNormalize(&outfrust.bottomPlane, &outfrust.bottomPlane);
		D3DXPlaneNormalize(&outfrust.nearPlane, &outfrust.nearPlane);
		D3DXPlaneNormalize(&outfrust.farPlane, &outfrust.farPlane);

	}
	void Frustum::calculateMatrixFromFrustrum(const Frustum& frust, D3DXMATRIX &matrix)
	{

	}
	

	void Frustum::convertToSimdFrustrum(Frustum::SIMDFrustrum& out) const
	{

		__declspec(align(16)) float line[4];

		line[0] = leftPlane.a;
		line[1] = rightPlane.a;
		line[2] = topPlane.a;
		line[3] = bottomPlane.a;

		out.x1x2x3x4 = _mm_load_ps(line);

		line[0] = leftPlane.b;
		line[1] = rightPlane.b;
		line[2] = topPlane.b;
		line[3] = bottomPlane.b;

		out.y1y2y3y4 = _mm_load_ps(line);

		line[0] = leftPlane.c;
		line[1] = rightPlane.c;
		line[2] = topPlane.c;
		line[3] = bottomPlane.c;

		out.z1z2z3z4 = _mm_load_ps(line);

		line[0] = leftPlane.d;
		line[1] = rightPlane.d;
		line[2] = topPlane.d;
		line[3] = bottomPlane.d;

		out.d1d2d3d4 = _mm_load_ps(line);

		line[0] = farPlane.a;
		line[1] = nearPlane.a;
		line[2] = farPlane.a;
		line[3] = nearPlane.a;

		out.x5x6x5x6 = _mm_load_ps(line);

		line[0] = farPlane.b;
		line[1] = nearPlane.b;
		line[2] = farPlane.b;
		line[3] = nearPlane.b;

		out.y5y6y5y6 = _mm_load_ps(line);

		line[0] = farPlane.c;
		line[1] = nearPlane.c;
		line[2] = farPlane.c;
		line[3] = nearPlane.c;

		out.z5z6z5z6 = _mm_load_ps(line);

		line[0] = farPlane.d;
		line[1] = nearPlane.d;
		line[2] = farPlane.d;
		line[3] = nearPlane.d;

		out.d5d6d5d6 = _mm_load_ps(line);

	}
    
}