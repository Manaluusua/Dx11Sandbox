#ifndef DX11SANDBOX_COMMONTYPES_H
#define DX11SANDBOX_COMMONTYPES_H

#include <string>
#include <windows.h>
namespace Dx11Sandbox
{
	//misc
	typedef std::basic_string<TCHAR> wstring;
    typedef std::string string;
	typedef unsigned int GIDTYPE;


	//RenderQueueID
	typedef unsigned int RenderQueueID;
	typedef unsigned int RenderMask;

	const RenderQueueID RENDERQUEUE_FIRST = 0;
	const RenderQueueID RENDERQUEUE_TERRAIN = 500;
	const RenderQueueID RENDERQUEUE_DEFAULT_OPAQUE = 1000;
	const RenderQueueID RENDERQUEUE_BEFORELIGHTPASS = 1500;
	const RenderQueueID RENDERQUEUE_AFTERLIGHTPASS = 1501;
	const RenderQueueID RENDERQUEUE_SKYBOX = 2000;
	const RenderQueueID RENDERQUEUE_AFTER_OPAQUESCENE_DONE = 3000;
	const RenderQueueID RENDERQUEUE_TRANSPARENT = 4000;
	const RenderQueueID RENDERQUEUE_HUD = 5000;

	const RenderMask RENDERMASK_DEFAULT_OPAQUE = 0x1;
	const RenderMask RENDERMASK_LIGHTPASS = 0x2;
	const RenderMask RENDERMASK_SKYBOX = 0x4;
	const RenderMask RENDERMASK_TRANSPARENT = 0x8;
	const RenderMask RENDERMASK_HUD = 0x10;
}

#endif