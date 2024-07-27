#pragma once

#include "RenderSystem.h"
#include "DeviceContext.h"
#include <d3d11.h>
#include <memory>

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, ID3D11Device* device);
	void update(DeviceContext* context, void* buffer);
	~ConstantBuffer();
private:
	ID3D11Buffer* output_buffer; //output buffer
	ID3D11Device* device;
private:
	friend class DeviceContext;
	friend class App;
};