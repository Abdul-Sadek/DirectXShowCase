#pragma once

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include <vector>

enum CULL_MODE {
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class GraphicsEngine;

class Material
{
public:
	Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	~Material();
	void addTexture(Texture* texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size, ConstantBuffer* constant_buffer, DeviceContext* device_context_class);
	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();
private:
	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;
	ConstantBuffer* m_constant_buffer;
	std::vector<Texture*> m_textures;
	CULL_MODE m_cull_mode = CULL_MODE_BACK;
private:
	friend class GraphicsEngine;
};

