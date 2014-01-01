#ifndef DX11SANDBOX_FRUSTRUM_H
#define DX11SANDBOX_FRUSTRUM_H
#include "MathUtil.h"
#include <xmmintrin.h>
namespace Dx11Sandbox
{
    class Frustrum
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

    public:
		inline static UINT32 cullSpheresSSE(Frustrum::SIMDFrustrum& frust,  const D3DXVECTOR4& sphere1, const D3DXVECTOR4& sphere2 );

        Frustrum();
        ~Frustrum();

		inline void convertToSimdFrustrum(Frustrum::SIMDFrustrum& out) const;
        D3DXPLANE leftPlane;
        D3DXPLANE rightPlane;
        D3DXPLANE topPlane;
        D3DXPLANE bottomPlane;
        D3DXPLANE farPlane;
        D3DXPLANE nearPlane;
        


        
		
        
    };

    //inlines

    void Frustrum::convertToSimdFrustrum(Frustrum::SIMDFrustrum& out) const
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

	UINT32 Frustrum::cullSpheresSSE(Frustrum::SIMDFrustrum& frust,  const D3DXVECTOR4& sphere1, const D3DXVECTOR4& sphere2 )
	{
		UINT32 cullmask = 0x0;

		UINT32 bitmask = 0x1; 

		__m128 res1;
		__m128 res2;

		__m128 vec1_xxxx = _mm_set1_ps(sphere1.x);
		__m128 vec1_yyyy = _mm_set1_ps(sphere1.y);
		__m128 vec1_zzzz = _mm_set1_ps(sphere1.z);
		__m128 vec1_rrrr = _mm_set1_ps(-sphere1.w);
    
		__m128 vec2_xxxx = _mm_set1_ps(sphere2.x);
		__m128 vec2_yyyy = _mm_set1_ps(sphere2.y);
		__m128 vec2_zzzz = _mm_set1_ps(sphere2.z);
		__m128 vec2_rrrr = _mm_set1_ps(-sphere2.w);

		__m128 vec12_xxxx = _mm_set_ps(sphere1.x, sphere1.x, sphere2.x, sphere2.x);
		__m128 vec12_yyyy = _mm_set_ps(sphere1.y, sphere1.y, sphere2.y, sphere2.y);
		__m128 vec12_zzzz = _mm_set_ps(sphere1.z, sphere1.z, sphere2.z, sphere2.z);
		__m128 vec12_rrrr = _mm_set_ps(-sphere1.w, -sphere1.w, -sphere2.w, -sphere2.w);

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

        
