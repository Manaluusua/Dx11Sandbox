#include "Mesh.h"
#include "DXUT.h"
namespace Dx11Sandbox
{
    Mesh::Mesh()
        :m_primType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
        m_indices.format = DXGI_FORMAT_R16_UINT;
        m_indices.indexCount = 0;
        m_indices.buffer = 0;

        m_vertices.numVertices = 0;
        m_vertices.stride = 0;
        m_vertices.buffer = 0;
    }


    Mesh::~Mesh()
    {
        SAFE_RELEASE(m_vertices.buffer);
        SAFE_RELEASE(m_indices.buffer);
    }

    void Mesh::createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, UINT numVertices, UINT numIndices,
            DXGI_FORMAT indexFormat,MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        UINT indexSize = (indexFormat == DXGI_FORMAT_R16_UINT?2:4);
        BYTE *indexBuffer = new BYTE[indexSize*numIndices];

        int numBuffers = MeshInputLayouts::getElementCountForType(type);
        const UINT* bufferSizes = MeshInputLayouts::getElementSizesForType(type);

        assert(numBuffers>0);

        UINT stride = 0;
        for(int i=0;i<numBuffers;i++)
        {
            stride += bufferSizes[i];
        }

        BYTE* vertexBuffer = new BYTE[stride*numVertices];

        for(UINT i=0;i<numVertices;++i)
        {
            UINT elemOffset = 0;
            for(int j=0;j<numBuffers;++j)
            {
                UINT elemSize = bufferSizes[j];
                memcpy( &vertexBuffer[i*stride + elemOffset],  &vbuffers[j][i*elemSize], elemSize);
                elemOffset += elemSize;
            }
        }


        for(UINT i = 0; i< numIndices;++i)
        {
            memcpy(&indexBuffer[i*indexSize], &ibuffer[i*indexSize], indexSize);
        }
       
        commitMeshDataToDevice(device, vertexBuffer, stride, numVertices, indexBuffer, indexFormat, numIndices);

        delete [] vertexBuffer;
        delete [] indexBuffer;
  
    }



    bool Mesh::commitMeshDataToDevice(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices, void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage, UINT cpuAccess,  bool createSOBuffer)
    {

	    D3D11_BUFFER_DESC vertBuffDesc, indBuffDesc;
	    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	    HRESULT hr;

	    vertBuffDesc.Usage = usage;
	    vertBuffDesc.ByteWidth = stride*numVertices;
	    
        if(createSOBuffer)
        {
            vertBuffDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT;
        }
        else
        {
            vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        }

	    vertBuffDesc.CPUAccessFlags = cpuAccess;
	    vertBuffDesc.MiscFlags = 0;
	    vertBuffDesc.StructureByteStride = 0;

	    vertexData.pSysMem = vertices;
	    vertexData.SysMemPitch = 0;
	    vertexData.SysMemSlicePitch = 0;

        hr = device->CreateBuffer(&vertBuffDesc, &vertexData, &m_vertices.buffer);
	    if(FAILED(hr))
	    {
		    return false;
	    }

        m_vertices.numVertices = numVertices;
        m_vertices.stride = stride;

	    indBuffDesc.Usage = usage;
        indBuffDesc.ByteWidth = (indexFormat == DXGI_FORMAT_R16_UINT?2:4)*numIndices;
	    indBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    indBuffDesc.CPUAccessFlags = cpuAccess;
	    indBuffDesc.MiscFlags = 0;
	    indBuffDesc.StructureByteStride = 0;


	    indexData.pSysMem = indices;
	    indexData.SysMemPitch = 0;
	    indexData.SysMemSlicePitch = 0;

        hr = device->CreateBuffer(&indBuffDesc, &indexData, &m_indices.buffer);
	    if(FAILED(hr))
	    {
		    return false;
	    }
        m_indices.format = indexFormat;
        m_indices.indexCount = numIndices;
        
	    return true;
    }
}