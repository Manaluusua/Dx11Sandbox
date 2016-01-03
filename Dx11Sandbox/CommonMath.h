#ifndef DX11SANDBOX_COMMONMATH_H
#define DX11SANDBOX_COMMONMATH_H

#include <DirectXMath.h>

namespace Dx11Sandbox
{
	//TODO: Create proper types that properly hides the DirectXMaths underlying Simd types
	typedef DirectX::XMFLOAT2 Vec2;
	typedef DirectX::XMFLOAT3 Vec3;
	typedef DirectX::XMFLOAT4 Vec4;
	typedef DirectX::XMMATRIX Matrix;
	typedef DirectX::XMFLOAT4X4 Matrix4x4;
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
	inline void matMakeIdentity(Matrix& mat)
	{

	}

	inline void matMakeIdentity(Matrix4x4& mat)
	{

	}

	inline void matInverse(const Matrix& in, Matrix& out)
	{

	}

	inline void matrix4x4ToMatrix(const Matrix4x4& in, Matrix& out)
	{

	}

	inline void matrixToMatrix4x4(const Matrix& in, Matrix4x4& out)
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

	inline Quat multiplyQuat(const Quat& m1, const Quat& m2)
	{

	}

	inline void quatConjugate(const Quat& q, Quat& conj)
	{

	}



	//transforms
	inline void transformVec3Point(const Vec3& vec, const Matrix& matrix, Vec3& vectorOut)
	{

	}

	inline void transformVec3Direction(const Vec3& vec, const Matrix& matrix, Vec3& vectorOut)
	{

	}

	inline void transformVec4(const Vec4& vec, const Matrix& matrix, Vec4& vectorOut)
	{

	}

	inline void rotateVecByQuat(const Vec3& vec, const Quat& quat, Vec3& out)
	{

	}


	//creation of transformations, the order of the parameters imply the order of operations
	inline void createTransformationOST( const Quat& orientation, const Vec3& scale, const Vec3& translation, Matrix& outTransformation)
	{

	}

	inline void createTransformationOT(const Quat& orientation, const Vec3& translation, Matrix& outTransformation)
	{

	}

	inline void createTransformationTO(const Vec3& translation, const Quat& orientation, Matrix& outTransformation)
	{

	}

	inline void createTransformationOS(const Quat& orientation, const Vec3& scale, Matrix& outTransformation)
	{

	}

	inline void createTransformationST(const Vec3& scale, const Vec3& translation, Matrix& outTransformation)
	{

	}

	inline void createLookAtLH(const Vec3 &eye, const Vec3 &at, const Vec3 &axisUp, Matrix& outTransformation)
	{

	}

	inline void createPerspectiveProjLH(float fovy, float aspectRatio, float n, float f, Matrix& out)
	{

	}

	inline void createOrthoProjLH(float sizeY, float sizeX, float n, float f, Matrix& out)
	{

	}

	inline void createReflection(const Vec4& reflectionPlane, Matrix& out)
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

		inline void matToSimdMat(const Matrix4x4& in, Matrix& out)
		{

		}

		inline void simdMatToMat(const Matrix& in, Matrix4x4& out)
		{

		}

	}

}

#endif