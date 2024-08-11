#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	Mesh(const wchar_t* full_path, ID3D11Device* device, void* shader_byte_code, size_t size_shader);
	~Mesh();
	const VertexBuffer* getVertexBuffer();
	const IndexBuffer* getIndexBuffer();
private:
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
private:
	friend class DeviceContext;
	friend class App;
};

