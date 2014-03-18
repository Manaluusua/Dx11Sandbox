#include "MathUtil.h"
#include <cmath>

namespace Dx11Sandbox
{
    namespace MathUtil
    {
        D3DXVECTOR3 rotateVec3ByQuat(const D3DXVECTOR3 *vec, const D3DXQUATERNION *quat)
        {
            D3DXQUATERNION conj, res;
            D3DXQuaternionConjugate(&conj,quat);
            res = conj*D3DXQUATERNION(vec->x, vec->y, vec->z,1.f)*(*quat);
            return D3DXVECTOR3(res.x,res.y,res.z);
        }

		float randomFloat()
		{
			return static_cast<float>(rand()) / RAND_MAX;
		}


        void calculateOrthogonalVector(const D3DXVECTOR3& vec, D3DXVECTOR3& orthogonal)
        {
            
        float smallest = std::abs(vec.x);
        int smallestInd = 0;

        if(std::abs(vec.y) < smallest)
        {
            smallest = std::abs(vec.y);
            smallestInd = 1;
        }

        if(std::abs(vec.z) < smallest)
        {
            smallest = std::abs(vec.z);
            smallestInd = 2;
        }

        if(smallestInd == 0)
        {
            orthogonal.x = 0;
            orthogonal.y = -vec.z;
            orthogonal.z = vec.y;
        }else if(smallestInd == 1)
        {
            orthogonal.x = -vec.z;
            orthogonal.y = 0;
            orthogonal.z = vec.x;
        }else if(smallestInd == 2)
        {
            orthogonal.x = -vec.y;
            orthogonal.y = vec.x;
            orthogonal.z = 0;
        }


        }
    }
}