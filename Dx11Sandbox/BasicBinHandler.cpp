#include "BasicBinHandler.h"
#include "CullData.h"

#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include <D3D11.h>
#include "d3dx11effect.h"
#include "Camera.h"
#include "CullableGeometry.h"

namespace Dx11Sandbox
{
    BasicBinHandler::BasicBinHandler(void)
    {
    }


    BasicBinHandler::~BasicBinHandler(void)
    {
    }


	 RenderData** BasicBinHandler::setupForRendering(RenderData** objects, unsigned int objectCount, unsigned int *objectsOutCount, RenderContext* state)
	{
		*objectsOutCount = objectCount;
		return objects;
	}

	
}
