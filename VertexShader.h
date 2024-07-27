#pragma once

#include <d3d11.h>
#include <memory>

class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, ID3D11Device* device);
	~VertexShader();
private:
	ID3D11VertexShader* vertex_shader;
	ID3D11Device* device;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};

