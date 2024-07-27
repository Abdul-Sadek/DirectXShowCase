#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, ID3D11Device* device)
{
	if (!SUCCEEDED(device->CreateVertexShader(shader_byte_code, byte_code_size,
		nullptr, &vertex_shader))) {
		throw std::exception("Vertex shader not created successfully");
	}
}

VertexShader::~VertexShader()
{
	vertex_shader->Release();
}