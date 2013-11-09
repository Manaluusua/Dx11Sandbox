#include "BasicBinHandler.h"
#include "CullInfo.h"

#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include <D3D11.h>
#include <d3dx9math.h>
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


	void BasicBinHandler::setupForRendering(RenderData** objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state)
	{
		objectsOut = objects;
		*objectsOutCount = objectCount;
	}

	
}
