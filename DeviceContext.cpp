#include "DeviceContext.h"
#include <exception>
#include "imgui/imgui.h"


DeviceContext::DeviceContext(ID3D11DeviceContext* device_context) : device_context(device_context)
{
}

void DeviceContext::clearRenderTargetColor(const SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	// Ensure the swap_chain and its RTV are valid
	if (swap_chain == nullptr || swap_chain->getRenderTargetView() == nullptr) {
		throw std::exception("SwapChain or RenderTargetView is null");
	}

	// Get the render target view from the swap chain
	ID3D11RenderTargetView* rtv = swap_chain->getRenderTargetView();
	if (rtv == nullptr) {
		throw std::exception("RenderTargetView is null");
	}

	// Define the clear color
	float clearColor[4] = { red, green, blue, alpha };

	// Clear the render target view
	device_context->ClearRenderTargetView(rtv, clearColor);
}

void DeviceContext::clearDepthStencilView(ID3D11DepthStencilView* depth_stencil_view)
{
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeviceContext::setVertexBuffer(const VertexBuffer* vertex_buffer)
{
	if (vertex_buffer == nullptr) {
		throw std::exception("vertex_buffer is nullptr");
	}
	if (vertex_buffer->output_buffer == nullptr) {
		throw std::exception("vertex_buffer->output_buffer is nullptr");
	}

	if (vertex_buffer->input_layout == nullptr) {
		throw std::exception("vertex_buffer->input_layout is nullptr");
	}

	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer->output_buffer, &stride, &offset);
	device_context->IASetInputLayout(vertex_buffer->input_layout);
}

void DeviceContext::setIndexBuffer(const IndexBuffer* index_buffer)
{
	if (index_buffer == nullptr) {
		throw std::exception("index_buffer is nullptr");
	}

	if (index_buffer->output_buffer == nullptr) {
		throw std::exception("index_buffer->output_buffer is nullptr");
	}
	device_context->IASetIndexBuffer(index_buffer->output_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShader* vertex_shader)
{
	device_context->VSSetShader(vertex_shader->vertex_shader, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShader* pixel_shader)
{
	device_context->PSSetShader(pixel_shader->pixel_shader, nullptr, 0);
}

void DeviceContext::setConstantBuffer(const VertexShader* vertex_shader, const ConstantBuffer* buffer)
{
	device_context->VSSetConstantBuffers(0, 1, &buffer->output_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShader* pixel_shader, const ConstantBuffer* buffer)
{
	device_context->PSSetConstantBuffers(0, 1, &buffer->output_buffer);
}

void DeviceContext::setTexture(const VertexShader* vertex_shader, const std::vector<Texture*>& textures, unsigned int num_texture)
{
	ID3D11ShaderResourceView* list_res[32]{};
	ID3D11SamplerState* list_sampler[32]{};
	for (unsigned int i = 0; i < num_texture; i++)
	{
		list_res[i] = textures[i]->m_shader_res_view;
		list_sampler[i] = textures[i]->m_sampler_state;
	}
	device_context->VSSetShaderResources(0, num_texture, list_res);
	device_context->VSSetSamplers(0, num_texture, list_sampler);
}

void DeviceContext::setTexture(const PixelShader* pixel_shader, const std::vector<Texture*>& textures, unsigned int num_texture)
{
	ID3D11ShaderResourceView* list_res[32]{};
	ID3D11SamplerState* list_sampler[32]{};
	for (unsigned int i = 0; i < num_texture; i++)
	{
		list_res[i] = textures[i]->m_shader_res_view;
		list_sampler[i] = textures[i]->m_sampler_state;
	}
	device_context->PSSetShaderResources(0, num_texture, list_res);
	device_context->PSSetSamplers(0, num_texture, list_sampler);
}

DeviceContext::~DeviceContext()
{
	device_context->Release();
}