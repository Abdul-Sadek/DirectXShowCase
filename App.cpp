#include "App.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include <io.h>
#include <d3dcompiler.h>
#include <fstream>
#include <vector>


struct Vertex {
    Vector3D position;
    Vector4D color;
};
struct Constant
{
    DirectX::XMFLOAT4X4 worldViewProj;
};

Vertex vertices[] = {
    { Vector3D(-1.0f, -1.0f, -1.0f),    Vector4D(0.0f, 0.0f, 0.0f, 0.0f) },
    { Vector3D(-1.0f,  1.0f, -1.0f),    Vector4D(0.875f, 0.875f, 0.875f, 1.0f) },
    { Vector3D(1.0f,  1.0f, -1.0f),     Vector4D(0.75f, 0.75f, 0.75f, 1.0f) },
    { Vector3D(1.0f, -1.0f, -1.0f),     Vector4D(0.625f, 0.625f, 0.625f, 1.0f) },
    { Vector3D(-1.0f, -1.0f,  1.0f),    Vector4D(0.5f, 0.5f, 0.5f, 1.0f) },
    { Vector3D(-1.0f,  1.0f,  1.0f),    Vector4D(0.375f, 0.375f, 0.375f, 1.0f) },
    { Vector3D(1.0f,  1.0f,  1.0f),     Vector4D(0.25f, 0.25f, 0.25f, 1.0f) },
    { Vector3D(1.0f, -1.0f,  1.0f),     Vector4D(0.125f, 0.125f, 0.125f, 1.0f) },
};

UINT indices[] = {
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7,
    0, 1, 5, 0, 5, 4,
    1, 2, 6, 1, 6, 5,
    2, 3, 7, 2, 7, 6,
    3, 0, 4, 3, 4, 7
};

App::App()
{
}

App::~App()
{
}

// Utility function to load a compiled shader
std::vector<char> LoadShader(const std::wstring& filename) {
    std::ifstream shaderFile(filename, std::ios::binary);
    if (!shaderFile.is_open()) {
        throw std::exception("Failed to open shader file.");
    }
    return std::vector<char>((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
}

void App::onCreate()
{
    Window::onCreate();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Creating device and device context
    if (!graphicsEngine.createDevices(hwnd, 1280, 720)) {
        throw std::exception("Failed to create Direct3D device and swap chain");
    }

    g_pd3dDevice = graphicsEngine.device;
    g_pd3dDeviceContext = graphicsEngine.deviceContext;
    g_pSwapChain = graphicsEngine.swapChain->getSwapChain();
    graphicsEngine.swapChain->createRenderTargetView();
    g_mainRenderTargetView = graphicsEngine.swapChain->getRenderTargetView();
    depth_stencil_view = graphicsEngine.depth_view;

    // Ensure deviceContext is assigned correctly
    deviceContext = new DeviceContext(g_pd3dDeviceContext);
    if (deviceContext == nullptr) {
        IMGUI_DEBUG_LOG("deviceContext is nullptr after creating devices");
    }

    // Load compiled vertex shader
    std::vector<char> vsBytecode = LoadShader(L"VertexShader.cso");
    vertex_shader = new VertexShader(vsBytecode.data(), vsBytecode.size(), g_pd3dDevice);
    // Load compiled pixel shader
    std::vector<char> psBytecode = LoadShader(L"PixelShader.cso");
    pixel_shader = new PixelShader(psBytecode.data(), psBytecode.size(), g_pd3dDevice);

    // Create vertex buffer
    try {
        vertex_buffer = new VertexBuffer(vertices, sizeof(Vertex), ARRAYSIZE(vertices), vsBytecode.data(), vsBytecode.size(), g_pd3dDevice);
        if (vertex_buffer == nullptr) {
            IMGUI_DEBUG_LOG("Vertex buffer is nullptr after creation");
        }
        IMGUI_DEBUG_LOG("Vertex buffer created successfully");
    }
    catch (const std::exception& e) {
        IMGUI_DEBUG_LOG("Failed to create Vertex buffer: %s", e.what());
        return;
    }

    // Create index buffer
    try {
        index_buffer = new IndexBuffer(indices, ARRAYSIZE(indices), g_pd3dDevice);
        if (index_buffer == nullptr) {
            IMGUI_DEBUG_LOG("Index buffer is nullptr after creation");
        }
        IMGUI_DEBUG_LOG("Index buffer created successfully");
    }
    catch (const std::exception& e) {
        IMGUI_DEBUG_LOG("Failed to create Index buffer: %s", e.what());
        return;
    }

    // Set the buffers in the device context
    try {
        deviceContext->setVertexBuffer(vertex_buffer);
        IMGUI_DEBUG_LOG("Vertex buffer set successfully");
        deviceContext->setIndexBuffer(index_buffer);
        IMGUI_DEBUG_LOG("Index buffer set successfully");
    }
    catch (const std::exception& e) {
        IMGUI_DEBUG_LOG("Failed to set Vertex and Index buffers: %s", e.what());
    }
    //create the constant buffer
    Constant c;
    constant_buffer = new ConstantBuffer(&c, sizeof(Constant), g_pd3dDevice);
    deviceContext->setConstantBuffer(vertex_shader, constant_buffer);
    deviceContext->setConstantBuffer(pixel_shader, constant_buffer);

    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd)) {
        throw std::exception("Failed to initialize ImGui Win32 backend");
    }

    if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)) {
        throw std::exception("Failed to initialize ImGui DX11 backend");
    }
    // Create view and perspective matrices
    CreateViewAndPerspective();
}

void App::onUpdate()
{
    // (Your code process and dispatch Win32 messages)
    // Update the model transformation
    updateModel();
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)
    // Rendering
    render();

    // (Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App::onDestroy()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    graphicsEngine.release();

    Window::onDestroy();
}

void App::onFocus() {}
void App::onKillFocus() {}
void App::onSize() {}

void App::render() {
    deviceContext->clearRenderTargetColor(graphicsEngine.swapChain, 1.0f, 1.0f, 1.0f, 1.0f);
    deviceContext->clearDepthStencilView(depth_stencil_view);
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView,depth_stencil_view);
    deviceContext->setViewportSize(1280, 720);
    deviceContext->setVertexShader(vertex_shader);
    deviceContext->setPixelShader(pixel_shader);
    deviceContext->setVertexBuffer(vertex_buffer);
    deviceContext->setIndexBuffer(index_buffer);

    ImVec2 cursorPos = ImGui::GetCursorPos(); // Get the cursor position
    Vector3D translation(cursorPos.x, cursorPos.y, 0.0f); // Convert ImVec2 to Vector3D

    DirectX::XMMATRIX g_Player;
    // Initialize the world matrix
    g_Player = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationZ(m_rot_z);
    DirectX::XMMATRIX mTranslate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
    g_Player = mScale * mRotate * mTranslate;

    // Combine with view and projection matrices
    DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&view); // Load view matrix
    DirectX::XMMATRIX projMatrix = DirectX::XMLoadFloat4x4(&proj); // Load projection matrix
    DirectX::XMMATRIX worldViewProjMatrix = g_Player * viewMatrix * projMatrix; // Combine matrices


    Constant cb1;
    DirectX::XMStoreFloat4x4(&cb1.worldViewProj, DirectX::XMMatrixTranspose(worldViewProjMatrix));

    constant_buffer->update(deviceContext, &cb1);

    deviceContext->drawIndexedTriangleList(ARRAYSIZE(indices),0,0);

    // Render ImGui UI
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0);
}

void App::update() {}
void App::updateModel() {
    // Rotate the cube 1 degree per frame.
    Constant cb;
    DirectX::XMStoreFloat4x4(
        &cb.worldViewProj,
        DirectX::XMMatrixTranspose(
            DirectX::XMMatrixRotationY(
                DirectX::XMConvertToRadians(
                    (float)m_frame++
                )
            )
        )
    );

    if (m_frame == MAXUINT)  m_frame = 0;
}
void App::updateCamera() {}
void App::updateSkybox() {}
void App::CreateViewAndPerspective()
{
    // Use DirectXMath to create view and perspective matrices.
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eye, at, up);
    DirectX::XMStoreFloat4x4(&view, viewMatrix);

    float aspectRatio = 1280.0f / 720.0f;
    DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, aspectRatio, 0.01f, 100.0f);
    DirectX::XMStoreFloat4x4(&proj, projMatrix);
}