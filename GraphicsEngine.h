#pragma once

#include "DeviceContext.h"
#include "SwapChain.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Material.h"
#include <d3d11.h>
#include <memory>

class GraphicsEngine
{
private:
	//Initialize the GraphicsEngine and DirectX 11 Device
	GraphicsEngine();
	//Release all the resources loaded
	~GraphicsEngine();
public:
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);
	bool createDevices(HWND hwnd, UINT width, UINT height);
public:
	static GraphicsEngine* get();
	static void create();
	static void release();
public:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11DepthStencilView* depth_view = nullptr;
	SwapChain* swapChain = nullptr;
	DeviceContext* deviceContextClass = nullptr;
private:
	static GraphicsEngine* graphics_engine;
	unsigned char mesh_layout_byte_code[2048];
	size_t mesh_layout_size = 0;
private:
	friend class App;
	friend class Mesh;
};