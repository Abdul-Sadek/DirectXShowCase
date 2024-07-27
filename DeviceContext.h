#pragma once

#include "RenderSystem.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3d11.h>
#include <memory>

class VertexShader;
class SwapChain;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	void clearRenderTargetColor(const SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void clearDepthStencilView(ID3D11DepthStencilView* depth_stencil_view);
	void setVertexBuffer(const VertexBuffer* vertex_buffer);
	void setIndexBuffer(const IndexBuffer* index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(const VertexShader* vertex_shader);
	void setPixelShader(const PixelShader* pixel_shader);

	void setConstantBuffer(const VertexShader* vertex_shader, const ConstantBuffer* buffer);
	void setConstantBuffer(const PixelShader* pixel_shader, const ConstantBuffer* buffer);

	~DeviceContext();
private:
	ID3D11DeviceContext* device_context;
	ID3D11DepthStencilView* depth_stencil_view;
private:
	friend class ConstantBuffer;
};

