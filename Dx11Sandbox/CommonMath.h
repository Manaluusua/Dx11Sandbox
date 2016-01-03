#ifndef DX11SANDBOX_COMMONMATH_H
#define DX11SANDBOX_COMMONMATH_H

#include <DirectXMath.h>

namespace Dx11Sandbox
{
	//TODO: Create proper types that properly uses the DirectXMaths underlying Simd implementation
	typedef DirectX::XMFLOAT2 Vec2;
	typedef DirectX::XMFLOAT3 Vec3;
	typedef DirectX::XMFLOAT4 Vec4;
	typedef DirectX::XMFLOAT4X4 Mat4x4;
	typedef DirectX::XMVECTOR Quat;
	typedef DirectX::XMVECTOR SimdVec;



	//vec operators
	inline void vecCrossProduct(const Vec3& v1, const Vec3& v2, Vec3& out)
	{

	}

	inline void vecNormalize(const Vec2& v, Vec2& out)
	{

	}

	inline void vecNormalize(const Vec3& v, Vec3& out)
	{

	}

	inline void vecNormalize(const Vec4& v, Vec4& out)
	{

	}

	inline float vecLengthSqr(const Vec2& v)
	{

	}
	inline float vecLengthSqr(const Vec3& v)
	{

	}
	inline float vecLengthSqr(const Vec4& v)
	{

	}

	inline float vecDotProduct(const Vec3& v1, const Vec3 v2)
	{

	}

	//mat operators
	inline void matMakeIdentity(Mat4x4& mat)
	{

	}
	inline void matInverse(const Mat4x4& in, Mat4x4& out)
	{

	}

	

	//quat operators
	inline void quatMakeIdentity(Quat& quat)
	{

	}

	inline Quat createQuat(Vec3& axis, float angle)
	{

	}

	inline Quat createQuat(float x, float y, float z, float w)
	{

	}

	inline void quatConjugate(const Quat& q, Quat& conj)
	{

	}


	//transforms
	inline void transformVec3Point(const Vec3& vec, const Mat4x4& matrix, Vec3& vectorOut)
	{

	}

	inline void transformVec3Direction(const Vec3& vec, const Mat4x4& matrix, Vec3& vectorOut)
	{

	}

	inline void transformVec4(const Vec4& vec, const Mat4x4& matrix, Vec4& vectorOut)
	{

	}

	inline void rotateVecByQuat(const Vec3& vec, const Quat& quat, Vec3& out)
	{

	}


	//creation of transformations, the order of the parameters imply the order of operations
	inline void createTransformationOST( const Quat& orientation, const Vec3& scale, const Vec3& translation, Mat4x4& outTransformation)
	{

	}

	inline void createTransformationOT(const Quat& orientation, const Vec3& translation, Mat4x4& outTransformation)
	{

	}

	inline void createTransformationTO(const Vec3& translation, const Quat& orientation, Mat4x4& outTransformation)
	{

	}

	inline void createTransformationOS(const Quat& orientation, const Vec3& scale, Mat4x4& outTransformation)
	{

	}

	inline void createTransformationST(const Vec3& scale, const Vec3& translation, Mat4x4& outTransformation)
	{

	}

	inline void createLookAtLH(const Vec3 &eye, const Vec3 &at, const Vec3 &axisUp, Mat4x4& outTransformation)
	{

	}

	inline void createPerspectiveProjLH(float fovy, float aspectRatio, float near, float far, Mat4x4& out)
	{

	}

	inline void createOrthoProjLH(float sizeY, float sizeX, float near, float far, Mat4x4& out)
	{

	}

	inline void createReflection(const Vec4& reflectionPlane, Mat4x4& out)
	{

	}

	//Misc
	inline void planeNormalize(const Vec4& plane, Vec4 planeOut)
	{

	}

	

	



	//operator overloads to make the new types act more like the old D3DX* types
	inline bool operator==(const Vec2& v1, const Vec2& v2)
	{

	}

	inline bool operator==(const Vec3& v1, const Vec3& v2)
	{

	}

	inline bool operator==(const Vec4& v1, const Vec4& v2)
	{

	}

	inline bool operator==(const Quat& v1, const Quat& v2)
	{

	}

	inline Vec2 operator-(const Vec2& v1)
	{

	}

	inline Vec3 operator-(const Vec3& v1)
	{

	}

	inline Vec4 operator-(const Vec4& v1)
	{

	}


	inline Vec2 operator+(const Vec2& v1, const Vec2& v2)
	{

	}

	inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
	{

	}

	inline Vec4 operator+(const Vec4& v1, const Vec4& v2)
	{

	}

	inline Vec2 operator-(const Vec2& v1, const Vec2& v2)
	{

	}

	inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
	{

	}

	inline Vec4 operator-(const Vec4& v1, const Vec4& v2)
	{

	}

	inline void operator+=(Vec2& v1, const Vec2& v2)
	{

	}

	inline void operator+=(Vec3& v1, const Vec3& v2)
	{

	}

	inline void operator+=(Vec4& v1, const Vec4& v2)
	{

	}

	inline void operator-=(Vec2& v1, const Vec2& v2)
	{

	}

	inline void operator-=(Vec3& v1, const Vec3& v2)
	{

	}

	inline void operator-=(Vec4& v1, const Vec4& v2)
	{

	}

	inline Vec2 operator*(const Vec2& v1, float s)
	{

	}

	inline Vec3 operator*(const Vec3& v1, float s)
	{

	}

	inline Vec4 operator*(const Vec4& v1, float s)
	{

	}

	inline Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2)
	{

	}

	inline Quat operator*(const Quat& m1, const Quat& m2)
	{

	}

	//Vector register operations
	namespace SimdOperations
	{

		inline void vecToSimdVec(const Vec3& in, SimdVec& out)
		{

		}

		inline void simdVecToVec(const SimdVec& in, Vec3& out)
		{

		}

		inline void vecToSimdVec(const Vec4& in, SimdVec& out)
		{

		}

		inline void simdVecToVec(const SimdVec& in, Vec4& out)
		{

		}

		inline void matToSimdMat(const Mat4x4& in, SimdVec& out)
		{

		}

		inline void simdMatToMat(const SimdVec& in, Mat4x4& out)
		{

		}

		inline void quatToSimdVec(const Quat& in, SimdVec& out)
		{

		}

		inline void simdVecToQuat(const SimdVec& in, Quat& out)
		{

		}

	}

}

#endif