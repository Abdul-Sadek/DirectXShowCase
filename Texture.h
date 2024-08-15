#pragma once

#include <d3d11.h>

class Texture
{
public:
	Texture(const wchar_t* full_path, ID3D11Device* device, ID3D11DeviceContext* device_context);
	~Texture();
private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;
private:
	friend class DeviceContext;
	friend class App;
};

