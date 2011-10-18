#include "Mesh.h"
#include "DXUT.h"
#include "RenderContext.h"


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

        m_sharedVertices = false;
    }


    Mesh::~Mesh()
    {
        if(!m_sharedVertices)
        {
            SAFE_RELEASE(m_vertices.buffer);
        }
        SAFE_RELEASE(m_indices.buffer);

    }

    void Mesh::createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, UINT numVertices, UINT numIndices,
            DXGI_FORMAT indexFormat,MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        

        if(numVertices>0)
        {
            createVertexBuffer(device, vbuffers,numVertices,type);
        }

        if(numIndices>0)
        {
            createIndexBuffer(device,ibuffer,numIndices,indexFormat);
        }



  
    }


    void Mesh::createVertexBuffer(ID3D11Device* device,BYTE** vbuffers, UINT numVertices, MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        UINT stride = 0;
        BYTE* vertexBuffer= 0;

        if(numVertices>0)
        {
            int numBuffers = MeshInputLayouts::getElementCountForType(type);
            const UINT* bufferSizes = MeshInputLayouts::getElementSizesForType(type);

            assert(numBuffers>0);

           
            for(int i=0;i<numBuffers;i++)
            {
                stride += bufferSizes[i];
            }

            vertexBuffer = new BYTE[stride*numVertices];

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
        


            commitVertexDataToDevice(device, vertexBuffer, stride, numVertices);


            delete [] vertexBuffer;
        }

    }

    void Mesh::createIndexBuffer(ID3D11Device* device, BYTE* ibuffer, UINT numIndices, DXGI_FORMAT indexFormat)
    {
         UINT stride = 0;
        BYTE* vertexBuffer= 0;
        UINT indexSize = 0; 
        BYTE *indexBuffer = 0; 

        if(numIndices>0)
        {
            indexSize = (indexFormat == DXGI_FORMAT_R16_UINT?2:4);
            indexBuffer = new BYTE[indexSize*numIndices];

            for(UINT i = 0; i< numIndices;++i)
            {
                memcpy(&indexBuffer[i*indexSize], &ibuffer[i*indexSize], indexSize);
            }
        

            commitIndexDataToDevice(device, indexBuffer, indexFormat, numIndices);
        


            delete [] indexBuffer;
        }
    }


    bool Mesh::commitIndexDataToDevice(ID3D11Device* device,void* indices, DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage , UINT cpuAccess, bool createSOBuffer)
    {
        D3D11_BUFFER_DESC indBuffDesc;
	    D3D11_SUBRESOURCE_DATA indexData;
	    HRESULT hr;

        if(numIndices>0)
        {
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
        
	    return false;
    }

    bool Mesh::commitVertexDataToDevice(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices, D3D11_USAGE usage, UINT cpuAccess,  bool createSOBuffer)
    {

	    D3D11_BUFFER_DESC vertBuffDesc;
	    D3D11_SUBRESOURCE_DATA vertexData;
	    HRESULT hr;

        if(numVertices>0)
        {
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
            return true;
        }
        return false;
       
    }

    bool Mesh::bind(RenderContext *context)
    {
        Mesh* boundMesh = context->getBoundMesh();
        if(!boundMesh || &boundMesh->getVertexBuffer() != &m_vertices)
        {
            UINT* strides = &m_vertices.stride;
            UINT offsets[1];
            offsets[0] = 0;
            ID3D11Buffer *buffers[1];
            buffers[0] = m_vertices.buffer;
            context->getImmediateContext()->IASetVertexBuffers(0,1,buffers,strides, offsets);
        }
        if(!boundMesh || &boundMesh->getIndexBuffer() != &m_indices)
        {
            context->getImmediateContext()->IASetIndexBuffer(m_indices.buffer,m_indices.format, 0);
        }

        if(!boundMesh || boundMesh->getPrimType() != m_primType)
        {
            context->getImmediateContext()->IASetPrimitiveTopology( m_primType );
        }
        return true;
    }
}