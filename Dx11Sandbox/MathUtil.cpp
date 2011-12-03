#include "MathUtil.h"


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
    }
}