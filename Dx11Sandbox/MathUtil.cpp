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

		void calculateTangents(long vertexCount, const D3DXVECTOR3 *positions, const D3DXVECTOR3 *normals,
			const D3DXVECTOR2 *texCoords, long trianglesCount, const UINT *indices, D3DXVECTOR3 *outTangents)
		{

			ZeroMemory(outTangents, vertexCount * sizeof(D3DXVECTOR3));

			//calculate tangent for each triangle and accumulate that tangent for each vertices tangent
			for (long triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
			{
				
				long ind1 = indices[triangleIndex * 3];
				long ind2 = indices[triangleIndex * 3 + 1];
				long ind3 = indices[triangleIndex * 3 + 2];

				const D3DXVECTOR3& p1 = positions[ind1];
				const D3DXVECTOR3& p2 = positions[ind2];
				const D3DXVECTOR3& p3 = positions[ind3];

				const D3DXVECTOR2& t1 = texCoords[ind1];
				const D3DXVECTOR2& t2 = texCoords[ind2];
				const D3DXVECTOR2& t3 = texCoords[ind3];

				D3DXVECTOR3 dp1 = p2 - p1;
				D3DXVECTOR3 dp2 = p3 - p1;

				D3DXVECTOR2 dt1 = t2 - t1;
				D3DXVECTOR2 dt2 = t3 - t1;

				float denom = dt1.x * dt2.y - dt2.y * dt1.y;
				float r = denom == 0.f ? 0.f : 1.f / denom;

				//tangent
				D3DXVECTOR3 udir = D3DXVECTOR3((dt2.y * dp1.x - dt1.y * dp2.x) * r, (dt2.y * dp1.y - dt1.y * dp2.y) * r,
					(dt2.y * dp1.z - dt1.y * dp2.z) * r);

				//bitangent
				/*D3DXVECTOR3 vdir = D3DXVECTOR3((dt1.x * dp2.x - dt2.x * dp1.x) * r, (dt1.x * dp2.y - dt2.x * dp1.y) * r,
					(dt1.x * dp2.z - dt2.x * dp1.z) * r);*/

				outTangents[ind1] += udir;
				outTangents[ind2] += udir;
				outTangents[ind3] += udir;
				
			}

			//orthogonalize and normalize tangent
			for (long i = 0; i < vertexCount; ++i){
				const D3DXVECTOR3& normal = normals[i];
				const D3DXVECTOR3& tangent = outTangents[i];
				float d = D3DXVec3Dot(&normal, &tangent);
				D3DXVECTOR3 newTangent = tangent - d * normal;
				D3DXVec3Normalize(&newTangent, &newTangent);
				outTangents[i] = newTangent;
			}

		}

    }
}