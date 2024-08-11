#pragma once

#include <d3d11.h>
#include <memory>

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(void* list_indices, UINT size_list, ID3D11Device* device);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	ID3D11Buffer* output_buffer; //output buffer
	ID3D11Device* device;
private:
	friend class DeviceContext;
	friend class App;
	friend class Mesh;
};