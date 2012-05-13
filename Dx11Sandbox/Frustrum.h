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

        Frustrum();
        ~Frustrum();

        D3DXPLANE leftPlane;
        D3DXPLANE rightPlane;
        D3DXPLANE topPlane;
        D3DXPLANE bottomPlane;
        D3DXPLANE farPlane;
        D3DXPLANE nearPlane;
        
        void convertToSimdFrustrum(Frustrum::SIMDFrustrum& out) const;

        
    };

    //inlines

    inline void Frustrum::convertToSimdFrustrum(Frustrum::SIMDFrustrum& out) const
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

#endif

        
