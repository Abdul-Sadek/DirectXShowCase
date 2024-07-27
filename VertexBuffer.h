#pragma once

#include <d3d11.h>
#include <memory>

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
		void* shader_byte_code, UINT size_byte_shader, ID3D11Device* device);
	VertexBuffer();
	UINT getSizeVertexList();
	~VertexBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* output_buffer; //output buffer
	ID3D11InputLayout* input_layout; //input layout
	ID3D11Device* device;
private:
	friend class DeviceContext;
	friend class App;
};