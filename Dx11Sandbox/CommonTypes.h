#ifndef DX11SANDBOX_COMMONTYPES_H
#define DX11SANDBOX_COMMONTYPES_H

#include <string>
#include <windows.h>
namespace Dx11Sandbox
{
	//misc
	typedef std::basic_string<WCHAR> wstring;
    typedef std::string string;
	typedef unsigned int GIDTYPE;


	//RenderQueueID
	typedef unsigned int RenderQueueID;
	typedef unsigned int RenderLayer;

	const RenderQueueID RENDERQUEUE_FIRST = 0;
	const RenderQueueID RENDERQUEUE_TERRAIN = 500;
	const RenderQueueID RENDERQUEUE_DEFAULT_OPAQUE = 1000;
	const RenderQueueID RENDERQUEUE_OPAQUE_SCENE_INPUT = 2000;
	const RenderQueueID RENDERQUEUE_BEFORELIGHTPASS = 3000;
	const RenderQueueID RENDERQUEUE_AFTERLIGHTPASS = 3001;
	const RenderQueueID RENDERQUEUE_SKYBOX = 4000;
	const RenderQueueID RENDERQUEUE_TRANSPARENT = 5000;
	const RenderQueueID RENDERQUEUE_HUD = 6000;

	const RenderLayer RENDERLAYER_DEFAULT_OPAQUE = 0x1;
	const RenderLayer RENDERLAYER_LIGHTPASS = 0x2;
	const RenderLayer RENDERLAYER_SKYBOX = 0x4;
	const RenderLayer RENDERLAYER_OPAQUE_SCENE_INPUT = 0x8;
	const RenderLayer RENDERLAYER_TRANSPARENT = 0x10;
	const RenderLayer RENDERLAYER_HUD = 0x20;
}

#endif