#pragma once

#include "RenderSystem.h"
#include <d3d11.h>
#include <memory>

class PixelShader
{
public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, ID3D11Device* device);
	~PixelShader();
private:
	ID3D11PixelShader* pixel_shader;
	ID3D11Device* device;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};

