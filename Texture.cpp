#include "Texture.h"
#include "GraphicsEngine.h"
#include <DirectXTex.h>

Texture::Texture(const wchar_t* full_path, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	if (FAILED(res)) {
		throw std::exception("Failed to load image data!");
	}
	if (SUCCEEDED(res)) {
		res = DirectX::CreateTexture(device, image_data.GetImages(),
			image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
		if (FAILED(res)) throw std::exception("Texture not created properly!"); //exception check
		//shader resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;
		//sampler state description
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = (UINT)image_data.GetMetadata().mipLevels;
		//creating the sampler state (for different kinds of texture sampling)
		res = device->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res)) throw std::exception("Texture not created properly!"); //exception check
		//creating the shader resource view
		res = device->CreateShaderResourceView(m_texture, &desc,
			&m_shader_res_view);
		if (FAILED(res)) throw std::exception("Texture not created properly!"); //exception check
	}
	else {
		throw std::exception("Texture not created properly!");
	}
}

Texture::~Texture()
{
	m_shader_res_view->Release();
	m_texture->Release();
}
