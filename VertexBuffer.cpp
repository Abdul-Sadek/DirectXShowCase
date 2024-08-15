#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
	void* shader_byte_code, UINT size_byte_shader, ID3D11Device* device) : device(device), input_layout(nullptr), output_buffer(nullptr)
{
	//creating the buffer description
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_vertex * size_list;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	//creating subresource data
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;
	m_size_vertex = size_vertex;
	m_size_list = size_list;
	//creating the actual buffer
	if (FAILED(device->CreateBuffer(&buffer_desc, &init_data, &output_buffer))) {
		throw std::exception("Vertex buffer not created!");
	}
	//layout descriptor object
	
	//semantic name - semantic index - format - input slot - aligned byte offset - input slot class - instance data step rate
	D3D11_INPUT_ELEMENT_DESC layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT size_layout = ARRAYSIZE(layout);
	//creating the actual input layout
	if (FAILED(device->CreateInputLayout(layout, size_layout,
		shader_byte_code, size_byte_shader, &input_layout))) {
		throw std::exception("Input layout not created!");
	}
}

VertexBuffer::VertexBuffer()
{
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

VertexBuffer::~VertexBuffer()
{
	input_layout->Release();
	output_buffer->Release();
}
