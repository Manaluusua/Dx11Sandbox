#ifndef DX11SANDBOX_ILDESCRIPTIONS_H
#define DX11SANDBOX_ILDESCRIPTIONS_H
#include <D3D11.h>


namespace Dx11Sandbox
{
    namespace MeshInputLayouts
    {
        enum MESH_LAYOUT_TYPE
        {
            POS3NORM3TEX2,
            POS3NORM3TEX3,
            POS3TEX3,
            POS3TEX2,
			POS3NORM3,
			POS3,
            NONE
        };



        //Position 3, Normal 3, Tex 2
        const D3D11_INPUT_ELEMENT_DESC POS3NORM3TEX2_DESC[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        } ;
        const UINT POS3NORM3TEX2_ELEM_NUM = 3;
        const UINT POS3NORM3TEX2_ELEM_SIZES[] = {12,12,8};

        //Position3, Normal 3, Tex 3
        const D3D11_INPUT_ELEMENT_DESC POS3NORM3TEX3_DESC[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        } ;
        const UINT POS3NORM3TEX3_ELEM_NUM = 3;
        const UINT POS3NORM3TEX3_ELEM_SIZES[] = {12,12,12};

        //Position3, Tex 3
        const D3D11_INPUT_ELEMENT_DESC POS3TEX3_DESC[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        } ;
        const UINT POS3TEX3_ELEM_NUM = 2;
        const UINT POS3TEX3_ELEM_SIZES[] = {12,12};

        //Position3, tex 2
        const D3D11_INPUT_ELEMENT_DESC POS3TEX2_DESC[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        } ;

        const UINT POS3TEX2_ELEM_NUM = 2;
        const UINT POS3TEX2_ELEM_SIZES[] = {12,8};

		//Position3, normal3
		const D3D11_INPUT_ELEMENT_DESC POS3NORM3_DESC[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const UINT POS3NORM3_ELEM_NUM = 2;
		const UINT POS3NORM3_ELEM_SIZES[] = { 12, 12 };

		//Position3
		const D3D11_INPUT_ELEMENT_DESC POS3_DESC[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		const UINT POS3_ELEM_NUM = 1;
		const UINT POS3_ELEM_SIZES[] = { 12 };

        const D3D11_INPUT_ELEMENT_DESC* getElementDescForType(MESH_LAYOUT_TYPE type);
        const UINT getElementCountForType(MESH_LAYOUT_TYPE type);
        const UINT* getElementSizesForType(MESH_LAYOUT_TYPE type);

    }
}
#endif