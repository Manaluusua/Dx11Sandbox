#include "MathUtil.h"
#include <cstdlib>

namespace Dx11Sandbox
{
    namespace MathUtil
	{
		float randomFloat()
		{
			return static_cast<float>(rand()) / RAND_MAX;
		}


        void calculateOrthogonalVector(const Vec3& vec, Vec3& orthogonal)
        {
            
        float smallest = std::abs(vec[0]);
        int smallestInd = 0;

        if(std::abs(vec[1]) < smallest)
        {
            smallest = std::abs(vec[1]);
            smallestInd = 1;
        }

        if(std::abs(vec[2]) < smallest)
        {
            smallest = std::abs(vec[2]);
            smallestInd = 2;
        }

        if(smallestInd == 0)
        {
            orthogonal[0] = 0;
            orthogonal[1] = -vec[2];
            orthogonal[2] = vec[1];
        }else if(smallestInd == 1)
        {
            orthogonal[0] = -vec[2];
            orthogonal[1] = 0;
            orthogonal[2] = vec[0];
        }else if(smallestInd == 2)
        {
            orthogonal[0] = -vec[1];
            orthogonal[1] = vec[0];
            orthogonal[2] = 0;
        }


        }


		void calculateTangentAndBitangent( const Vec3& p1, const Vec3& p2, const Vec3& p3,
			const Vec2 &t1, const Vec2 &t2, const Vec2 &t3, Vec3 &outTangent, Vec3 &outBitangent)
		{


			Vec3 dp1 = p2 - p1;
			Vec3 dp2 = p3 - p1;

			Vec2 dt1 = t2 - t1;
			Vec2 dt2 = t3 - t1;

			float denom = dt1[0] * dt2[1] - dt2[0] * dt1[1];
			float r = denom == 0.f ? 0.f : 1.f / denom;

			outTangent = Vec3((dt2[1] * dp1[0] - dt1[1] * dp2[0]) * r, (dt2[1] * dp1[1] - dt1[1] * dp2[1]) * r,
				(dt2[1] * dp1[2] - dt1[1] * dp2[2]) * r);
			outBitangent = Vec3((dt1[0] * dp2[0] - dt2[0] * dp1[0]) * r, (dt1[0] * dp2[1] - dt2[0] * dp1[1]) * r,
			(dt1[0] * dp2[2] - dt2[0] * dp1[2]) * r);

		}

		void orthogonalizeAndNormalizeTangent(const Vec3 &tangent, const Vec3& normal, Vec3& tangentOut){
			float d = dot(normal, tangent);
			
			Vec3 newTangent = tangent - (normal * d);
			normalize(newTangent);
			tangentOut = newTangent;
			
		}

		float calculateHandedness(const Vec3 &tangent, const Vec3& bitangent, const Vec3& normal){
			Vec3 crossProd;
			cross(crossProd, normal, tangent);
			return dot(crossProd, bitangent) < 0 ? -1.f : 1.f;
			
		}

		void calculateTangents(long vertexCount, const Vec3 *positions, const Vec3 *normals,
			const Vec2 *texCoords, long trianglesCount, const unsigned int *indices, Vec4 *outTangents)
		{
			
			Vec3* tangents = new Vec3[vertexCount];
			Vec3* bitangents = new Vec3[vertexCount];

			for (long i = 0; i < vertexCount; ++i){
				tangents[i] = Vec3(0.f, 0.f, 0.f);
				bitangents[i] = Vec3(0.f, 0.f, 0.f);
			}

			

			//calculate tangent for each triangle and accumulate that tangent for each vertices tangent
			for (long triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
			{
				
				long ind1 = indices[triangleIndex * 3];
				long ind2 = indices[triangleIndex * 3 + 1];
				long ind3 = indices[triangleIndex * 3 + 2];

				const Vec3& p1 = positions[ind1];
				const Vec3& p2 = positions[ind2];
				const Vec3& p3 = positions[ind3];

				const Vec2& t1 = texCoords[ind1];
				const Vec2& t2 = texCoords[ind2];
				const Vec2& t3 = texCoords[ind3];

				

				//tangent
				Vec3 tangent;
				Vec3 bitangent;
				calculateTangentAndBitangent(p1, p2, p3, t1, t2, t3,tangent, bitangent);

				tangents[ind1] += tangent;
				tangents[ind2] += tangent;
				tangents[ind3] += tangent;

				bitangents[ind1] += bitangent;
				bitangents[ind2] += bitangent;
				bitangents[ind3] += bitangent;
				
			}

			//orthogonalize and normalize tangent
			for (long i = 0; i < vertexCount; ++i){
				const Vec3& normal = normals[i];
				Vec3& tangent = tangents[i];
				Vec3& bitangent = bitangents[i];
				Vec3 newTangent;
				orthogonalizeAndNormalizeTangent(tangent, normal, newTangent);
				outTangents[i] = Vec4(newTangent[0], newTangent[1], newTangent[2], calculateHandedness(tangent, bitangent, normal));
			}

			delete[] tangents;
			delete[] bitangents;
		}

    }
}