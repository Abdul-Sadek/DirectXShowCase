#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, ID3D11Device* device) : device(device), output_buffer(0)
{
	//creating the buffer description 
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = 4 * size_list;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = list_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	//creating the actual buffer
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &output_buffer))) {
		throw std::exception("Index buffer not created!");
	}
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}

IndexBuffer::~IndexBuffer()
{
	output_buffer->Release();
}
