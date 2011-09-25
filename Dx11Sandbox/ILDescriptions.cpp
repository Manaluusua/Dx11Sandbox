#include "ILDescriptions.h"


namespace Dx11Sandbox
{
    namespace MeshInputLayouts
    {

        const D3D11_INPUT_ELEMENT_DESC* getElementDescForType(MESH_LAYOUT_TYPE type)
        {
            switch(type)
            {
            case POS3NORM3TEX2:
                return POS3NORM3TEX2_DESC;
            case POS3NORM3TEX3:
                return POS3NORM3TEX3_DESC;
            case POS3TEX3:
                return POS3TEX3_DESC;

            default:
                return 0;
     
            }
        }

        const UINT getElementCountForType(MESH_LAYOUT_TYPE type)
        {
            switch(type)
            {
            case POS3NORM3TEX2:
                return POS3NORM3TEX2_ELEM_NUM;
            case POS3NORM3TEX3:
                return POS3NORM3TEX3_ELEM_NUM;
            case POS3TEX3:
                return POS3TEX3_ELEM_NUM;
            default:
                return 0;
     
            }
        }

        const UINT* getElementSizesForType(MESH_LAYOUT_TYPE type)
        {
            switch(type)
            {
            case POS3NORM3TEX2:
                return POS3NORM3TEX2_ELEM_SIZES;
            case POS3NORM3TEX3:
                return POS3NORM3TEX3_ELEM_SIZES;
            case POS3TEX3:
                return POS3TEX3_ELEM_SIZES;
            default:
                return 0;
     
            }
        }
    }
}