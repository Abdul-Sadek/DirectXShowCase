#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, ID3D11Device* device)
{
	//creating the buffer description
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_buffer;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	//creating subresource data
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;
	//creating the actual buffer
	if (FAILED(device->CreateBuffer(&buffer_desc, &init_data, &output_buffer))) {
		throw std::exception("Constant buffer not created!");
	}
}

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->device_context->UpdateSubresource(this->output_buffer, NULL, NULL, buffer,
		NULL, NULL);
}

ConstantBuffer::~ConstantBuffer()
{
	if (output_buffer)output_buffer->Release();
}
