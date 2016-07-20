#ifndef DX11SANDBOX_COMMONMATH_H
#define DX11SANDBOX_COMMONMATH_H

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>

using namespace gmtl;

namespace Dx11Sandbox
{

	//TODO: Create proper types
	typedef gmtl::Vec2f Vec2;
	typedef gmtl::Vec3f Vec3;
	typedef gmtl::Vec4f Vec4;
	typedef gmtl::Matrix44f Matrix;
	typedef gmtl::Quatf Quat;


	//mat operators
	inline void matMakeIdentity(Matrix& mat)
	{
		mat = gmtl::MAT_IDENTITY44F;
	}


	inline void matInverse(const Matrix& in, Matrix& out)
	{
		out = in;
		gmtl::invert(out);
	}



	

	//quat operators
	inline void quatMakeIdentity(Quat& quat)
	{
		quat = gmtl::QUAT_IDENTITYF;
	}

	inline Quat createQuatFromAxisAngle(float x, float y, float z, float angle)
	{
		float sinAngle = sin(angle / 2);

		return Quat(sinAngle * x, sinAngle * y, sinAngle * z, cos(angle / 2));
	}

	inline Quat createQuatFromAxisAngle(const Vec3& axis, float angle)
	{
		return createQuatFromAxisAngle(axis[0], axis[1], axis[2], angle);
	}

	inline Quat& multiplyQuat(const Quat& a, const Quat& b, Quat& out)
	{
		gmtl::mult(out,a, b);
		return out;
	}

	

	inline void quatConjugate(const Quat& q, Quat& conj)
	{
		conj = q;
		gmtl::conj(conj);
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

		Vec3 v(quat[0], quat[1], quat[2]);
		float s = quat[3];

		Vec3 temp;
		cross(temp, v, vec);

		
		out = 2.0f * dot(v, vec) * v
			+ (s*s - dot(v, v)) * vec
			+ 2.0f * s * temp;
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


	

	
}

#endif