#pragma once
#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_win32.h"
#include "imgui\\imgui_impl_dx11.h"
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Physics.h"
#include "CollisionShape.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include <d3d11.h>
#include <tchar.h>
#include <dxgi.h>
#include <exception>
#include <directxmath.h>
#include <vector>
#include <DirectXTex.h>

class App : public Window
{
public:
	App();
	~App();
	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;
public:
	void render();
	void update();
	void updateModel();
	void updateCamera();
	void updateSkybox();
	void CreateViewAndPerspective();
	void imgui_window_render_camera();
	void imgui_window_render_position();
	void imgui_window_render_rotation();
	void imgui_window_render_scale();
	void imgui_start_menu();
	void imgui_show_helper_window();
	void imgui_show_light_window();
	void imgui_show_physics_window();
private:
	GraphicsEngine graphicsEngine;
	Physics physics_engine;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	ID3D11Buffer* dx_index_buffer = nullptr;
	ID3D11Buffer* dx_vertex_buffer = nullptr;
	ID3D11Buffer* dx_constant_buffer = nullptr;
	ID3D11InputLayout* vertexLayout = nullptr;
	ID3D11DepthStencilView* depth_stencil_view = nullptr;
	ID3D11RasterizerState* pRasterState = nullptr;
private:
	SwapChain* swapChain = nullptr;
	DeviceContext* deviceContext = nullptr;
	const IndexBuffer* index_buffer = nullptr;
	const VertexBuffer* vertex_buffer = nullptr;
	ConstantBuffer* constant_buffer = nullptr;
	VertexShader* vertex_shader = nullptr;
	PixelShader* pixel_shader = nullptr;
private:
	struct MeshInstance {
		Mesh* mesh;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;
		std::vector<Texture*> textures; // List of textures for this mesh
		RigidBody* rigidbody;
		CollisionShape* collision_shape;
	};
	std::vector<MeshInstance> meshInstances;
private:
	bool g_SwapChainOccluded = false;
	UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
private:
	float old_delta;
	float new_delta;
	float delta_time;

	float delta_pos;
	float delta_scale;
	float delta_rot;

	UINT m_frame = 0;
	DirectX::XMFLOAT4X4 view = {};
	DirectX::XMFLOAT4X4 proj = {};

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;
	float m_rot_z = 0.0f;

	
	bool showWindowCamera = false;
	bool showWindowPosition = false;
	bool showWindowRotation = false;
	bool showWindowScale = false;
	bool showWindowLight = false;
	bool showHelperMenu = false;
	bool showMenu = true;
	bool gameStarted = false;
	bool quit_game = false;
	bool options_shown = false;

	float light_pos_x = 0.0f;
	float light_pos_y = 1.0f;
	float light_pos_z = 0.0f;

	float light_dir_x = 1.0f;
	float light_dir_y = 1.0f;
	float light_dir_z = 1.0f;

	float m_light_color_r = 1.0f;
	float m_light_color_g = 1.0f;
	float m_light_color_b = 1.0f;

	float light_intensity = 1.0f;

	float forward = 0.0f;
	float rightward = 0.0f;
	float upward = 0.0f;

	bool face_culling = true;

	float m_time = 0.0f;

	float m_scale_x = 1.0f;
	float m_scale_y = 1.0f;
	float m_scale_z = 1.0f;

	DirectX::XMFLOAT3 gravity = DirectX::XMFLOAT3(0.0f, -9.81f, 0.0f);
	float air_resistance = 0.1f;
	float temp_y_value = 1.0f;
};

