#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, ID3D11Device* device)
{
	if (!SUCCEEDED(device->CreatePixelShader(shader_byte_code, byte_code_size,
		nullptr, &pixel_shader))) {
		throw std::exception("Pixel shader not created!");
	}
}

PixelShader::~PixelShader()
{
	pixel_shader->Release();
}