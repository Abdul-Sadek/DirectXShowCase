#include "Material.h"
#include "GraphicsEngine.h"
#include <stdexcept>

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	//shaders
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	//vertex shader
	m_vertex_shader;
	//error checking exception
	if (!m_vertex_shader) throw std::runtime_error("Material not created successfully!");
	//pixel shader
	m_pixel_shader;
	//error checking exception
	if (!m_pixel_shader) throw std::runtime_error("Material not created successfully!");
}

Material::~Material()
{
}

void Material::addTexture(Texture* texture)
{
	m_textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_textures.size() - 1) return;
	m_textures.erase(m_textures.begin() + index - 1);
}

void Material::setData(void* data, unsigned int size, ConstantBuffer* constant_buffer, DeviceContext* device_context_class)
{
	if (!constant_buffer) {
		m_constant_buffer = constant_buffer;
	}
	else {
		m_constant_buffer->update(device_context_class, data);
	}
}

void Material::setCullMode(CULL_MODE mode)
{
	m_cull_mode = mode;
}

CULL_MODE Material::getCullMode()
{
	return m_cull_mode;
}
