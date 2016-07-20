#ifndef DX11SANDBOX_FRUSTRUM_H
#define DX11SANDBOX_FRUSTRUM_H
#include "CommonMath.h"
#include "CommonTypes.h"
#include <xmmintrin.h>
namespace Dx11Sandbox
{
    class Frustum
    {
    public:

        //a SIMD type frustrum used in culling 
        __declspec(align(16)) struct SIMDFrustrum
        {
            __m128 x1x2x3x4;
            __m128 y1y2y3y4;
            __m128 z1z2z3z4;
            __m128 d1d2d3d4;
            __m128 x5x6x5x6;
            __m128 y5y6y5y6;
            __m128 z5z6z5z6;
            __m128 d5d6d5d6;
        };

		static uint32_t cullSpheresSSE(Frustum::SIMDFrustrum& frust,  const Vec4& sphere1, const Vec4& sphere2 );
		static void calculateFrustrumFromMatrix(const Matrix &mat, Frustum& outfrust);
		static void calculateMatrixFromFrustrum(const Frustum& frust, Matrix &matrix);


        Frustum();
        ~Frustum();

		void convertToSimdFrustrum(Frustum::SIMDFrustrum& out) const;
        Vec4 leftPlane;
		Vec4 rightPlane;
		Vec4 topPlane;
		Vec4 bottomPlane;
		Vec4 farPlane;
		Vec4 nearPlane;
        


        
		
        
    };

    

	inline uint32_t Frustum::cullSpheresSSE(Frustum::SIMDFrustrum& frust,  const Vec4& sphere1, const Vec4& sphere2 )
	{
		uint32_t cullmask = 0x0;

		uint32_t bitmask = 0x1;

		__m128 res1;
		__m128 res2;

		__m128 vec1_xxxx = _mm_set1_ps(sphere1[0]);
		__m128 vec1_yyyy = _mm_set1_ps(sphere1[1]);
		__m128 vec1_zzzz = _mm_set1_ps(sphere1[2]);
		__m128 vec1_rrrr = _mm_set1_ps(-sphere1[3]);
    
		__m128 vec2_xxxx = _mm_set1_ps(sphere2[0]);
		__m128 vec2_yyyy = _mm_set1_ps(sphere2[1]);
		__m128 vec2_zzzz = _mm_set1_ps(sphere2[2]);
		__m128 vec2_rrrr = _mm_set1_ps(-sphere2[3]);

		__m128 vec12_xxxx = _mm_set_ps(sphere1[0], sphere1[0], sphere2[0], sphere2[0]);
		__m128 vec12_yyyy = _mm_set_ps(sphere1[1], sphere1[1], sphere2[1], sphere2[1]);
		__m128 vec12_zzzz = _mm_set_ps(sphere1[2], sphere1[2], sphere2[2], sphere2[2]);
		__m128 vec12_rrrr = _mm_set_ps(-sphere1[3], -sphere1[3], -sphere2[3], -sphere2[3]);

		__m128 zero = _mm_set1_ps(0.0f);

		__m128 dot1 = _mm_add_ps(_mm_mul_ps(vec1_xxxx, frust.x1x2x3x4), frust.d1d2d3d4);
		dot1 = _mm_add_ps(_mm_mul_ps(vec1_yyyy, frust.y1y2y3y4), dot1);
		dot1 = _mm_add_ps(_mm_mul_ps(vec1_zzzz, frust.z1z2z3z4), dot1);

		__m128 dot2 = _mm_add_ps(_mm_mul_ps(vec2_xxxx, frust.x1x2x3x4), frust.d1d2d3d4);
		dot2 = _mm_add_ps(_mm_mul_ps(vec2_yyyy, frust.y1y2y3y4), dot2);
		dot2 = _mm_add_ps(_mm_mul_ps(vec2_zzzz, frust.z1z2z3z4), dot2);

		__m128 dot12 = _mm_add_ps(_mm_mul_ps(vec12_xxxx, frust.x5x6x5x6), frust.d5d6d5d6);
		dot12 = _mm_add_ps(_mm_mul_ps(vec12_yyyy, frust.y5y6y5y6), dot12);
		dot12 = _mm_add_ps(_mm_mul_ps(vec12_zzzz, frust.z5z6z5z6), dot12);

		dot1 = _mm_cmpgt_ps(vec1_rrrr,dot1);
		dot2 = _mm_cmpgt_ps(vec2_rrrr,dot2);
		dot12 = _mm_cmpgt_ps(vec12_rrrr,dot12 );

		__m128 final1 = _mm_unpackhi_ps(zero, dot12);
		__m128 final2 = _mm_unpacklo_ps(zero, dot12);
   

		final1 = _mm_or_ps(dot1, final1);
		final2 = _mm_or_ps(dot2, final2);

		_mm_store_ps(res1.m128_f32, final1);
		_mm_store_ps(res2.m128_f32, final2);

		cullmask |= ~(res1.m128_u32[0] | res1.m128_u32[1] | res1.m128_u32[2] | res1.m128_u32[3])&bitmask;
		cullmask |= ~(res2.m128_u32[0] | res2.m128_u32[1] | res2.m128_u32[2] | res2.m128_u32[3])&bitmask << 1;

    

		return cullmask;
	}








}

#endif

        
