#include "App.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include <iostream>
#include <d3dcompiler.h>
#include <fstream>
#include <DirectXTex.h>


struct Vertex {
    Vector4D position;
    Vector2D texcoord;
    Vector3D normal;
    Vector4D Color;
};
__declspec(align(16))
struct Constant
{
    DirectX::XMFLOAT4X4 worldViewProj;
};

Vertex vertices[] = {
    { Vector4D(-1.0f, -1.0f, -1.0f, 1.0f),    Vector2D(0.75f, 0.75f) ,  Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(-1.0f,  1.0f, -1.0f, 1.0f),    Vector2D(0.75f, 0.0f) ,   Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(1.0f,  1.0f, -1.0f, 1.0f),     Vector2D(0.0f, 0.75f) ,   Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(1.0f, -1.0f, -1.0f, 1.0f),     Vector2D(0.75f, 0.0f) ,   Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(-1.0f, -1.0f,  1.0f, 1.0f),    Vector2D(0.0f, 0.75f) ,   Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(-1.0f,  1.0f,  1.0f, 1.0f),    Vector2D(0.0f, 0.0f) ,    Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(1.0f,  1.0f,  1.0f, 1.0f),     Vector2D(0.75f, 0.75f) ,  Vector3D(0.75f, 0.75f,0.75f) },
    { Vector4D(1.0f, -1.0f,  1.0f, 1.0f),     Vector2D(0.75f, 0.75f) ,  Vector3D(0.75f, 0.75f,0.75f) },
};

UINT indices[] = {
    0, 1, 2,     0, 2, 3,
    4, 5, 6,     4, 6, 7,
    0, 1, 5,     0, 5, 4,
    1, 2, 6,     1, 6, 5,
    2, 3, 7,     2, 7, 6,
    3, 0, 4,     3, 4, 7
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
    graphicsEngine.create();
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
    /*try {
        vertex_buffer = new VertexBuffer(vertices, sizeof(Vertex), ARRAYSIZE(vertices), vsBytecode.data(), vsBytecode.size(), g_pd3dDevice);
        if (vertex_buffer == nullptr) {
            IMGUI_DEBUG_LOG("Vertex buffer is nullptr after creation");
        }
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
    }
    catch (const std::exception& e) {
        IMGUI_DEBUG_LOG("Failed to create Index buffer: %s", e.what());
        return;
    }
    deviceContext->setVertexBuffer(vertex_buffer);
    deviceContext->setIndexBuffer(index_buffer);
    */


    //create the constant buffer
    Constant c;
    constant_buffer = new ConstantBuffer(&c, sizeof(Constant), g_pd3dDevice);
    

    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd)) {
        throw std::exception("Failed to initialize ImGui Win32 backend");
    }

    if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)) {
        throw std::exception("Failed to initialize ImGui DX11 backend");
    }
    deviceContext->setViewportSize(1280, 720);

    mesh = new Mesh(L"Assets\\Meshes\\box.obj", g_pd3dDevice, vsBytecode.data(), vsBytecode.size());
    texture = new Texture(L"Assets\\Textures\\brick.png", g_pd3dDevice, g_pd3dDeviceContext);
    textures.push_back(texture);
    if (mesh == nullptr) {
        IMGUI_DEBUG_LOG("Mesh is null after creation");
    }

    /*D3D11_BLEND_DESC blend_desc;
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ID3D11BlendState* blendstate;
    g_pd3dDevice->CreateBlendState(&blend_desc,&blendstate);
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    g_pd3dDeviceContext->OMSetBlendState(blendstate, blendFactor, sampleMask);*/

    D3D11_RASTERIZER_DESC rasterDesc = {};
    ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthClipEnable = true;
    ID3D11RasterizerState* pRasterState = nullptr;
    g_pd3dDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
    g_pd3dDeviceContext->RSSetState(pRasterState);
    if (pRasterState) pRasterState->Release();

    deviceContext->setConstantBuffer(vertex_shader, constant_buffer);
    deviceContext->setConstantBuffer(pixel_shader, constant_buffer);
    //deviceContext->setTexture(pixel_shader, textures, textures.size());
    deviceContext->setVertexShader(vertex_shader);
    deviceContext->setPixelShader(pixel_shader);
    deviceContext->setVertexBuffer(mesh->getVertexBuffer());
    deviceContext->setIndexBuffer(mesh->getIndexBuffer());

    if (mesh->m_index_buffer->getSizeIndexList() < 1) {
        IMGUI_DEBUG_LOG("Mesh index buffer is empty after creation");
    }

    // Create view and perspective matrices
    CreateViewAndPerspective();
    
}

void App::onUpdate()
{
    // (Your code process and dispatch Win32 messages)
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(); // Show demo window! :)
    if (showMenu) {
        imgui_start_menu();
    }
    if (gameStarted) {
        //camera view window
        if (ImGui::IsKeyReleased(ImGuiKey_C)) {
            showWindowCamera = !showWindowCamera; // toggle the window visibility
        }
        if (showWindowCamera) {
            imgui_window_render_camera();
        }
        //position window
        if (ImGui::IsKeyReleased(ImGuiKey_P)) {
            showWindowPosition = !showWindowPosition; // toggle the window visibility
        }
        if (showWindowPosition) {
            imgui_window_render_position();
        }
        //rotation window
        if (ImGui::IsKeyReleased(ImGuiKey_R)) {
            showWindowRotation = !showWindowRotation; // toggle the window visibility
        }
        if (showWindowRotation) {
            imgui_window_render_rotation();
        }
        //scaling window
        if (ImGui::IsKeyReleased(ImGuiKey_Z)) {
            showWindowScale = !showWindowScale; // toggle the window visibility
        }
        if (showWindowScale) {
            imgui_window_render_scale();
        }

        // Rendering
        render();
    }
    // (Your code clears your framebuffer, renders your other stuff etc.)
    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    // Present the frame
    g_pSwapChain->Present(1, 0);

    if (quit_game || ImGui::IsKeyReleased(ImGuiKey_Escape)) {
        onDestroy();
    }
    
}

void App::onDestroy()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (vertex_buffer) {
        delete vertex_buffer;
    }
    if (index_buffer) {
        delete index_buffer;
    }
    if (vertex_shader) {
        delete vertex_shader;
    }
    if (pixel_shader) {
        delete pixel_shader;
    }
    if (mesh) {
        delete mesh;
    }
    /*if (texture) {
        delete texture;
    }*/

    graphicsEngine.release();

    Window::onDestroy();
}

void App::onFocus() {}
void App::onKillFocus() {}
void App::onSize() {}

void App::render() {

    deviceContext->clearRenderTargetColor(graphicsEngine.swapChain, 0.25f, 0.25f, 0.75f, 1.0f);
    deviceContext->clearDepthStencilView(depth_stencil_view);
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, depth_stencil_view);

    if (ImGui::IsKeyPressed(ImGuiKey_D)) {
        forward += 1.0f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_A)) {
        forward -= 1.0f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_W)) {
        rightward += 1.0f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_S)) {
        rightward -= 1.0f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
        upward += 1.0f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
        upward -= 1.0f;
    }
    DirectX::XMMATRIX g_Player;
    // Initialize the world matrix
    g_Player = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationRollPitchYaw(m_rot_y,m_rot_x,m_rot_z);
    DirectX::XMMATRIX mTranslate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
    g_Player = mScale * mRotate * mTranslate;

    DirectX::XMVECTOR eye = DirectX::XMVectorSet(5.0f + rightward, 5.0f + upward, 2.5f + forward, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eye, at, up);
    DirectX::XMMATRIX projMatrix = DirectX::XMLoadFloat4x4(&proj); // Load projection matrix
    DirectX::XMMATRIX worldViewProjMatrix = DirectX::XMMatrixIdentity();
    
    mRotate = DirectX::XMMatrixRotationRollPitchYaw(m_rot_y, m_rot_x, m_rot_z);
    mTranslate = DirectX::XMMatrixTranslation(x, y, z);
    mScale = DirectX::XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
    g_Player = mScale * mRotate * mTranslate;

    worldViewProjMatrix = g_Player * viewMatrix * projMatrix; // Combine matrices

    Constant cb1;
    DirectX::XMStoreFloat4x4(&cb1.worldViewProj, DirectX::XMMatrixTranspose(worldViewProjMatrix));
    constant_buffer->update(deviceContext, &cb1);
    //cube drawing
    //deviceContext->drawIndexedTriangleList(ARRAYSIZE(indices), 0, 0);
    //mesh drawing
    deviceContext->drawIndexedTriangleList(mesh->getIndexBuffer()->m_size_list, 0, 0);


}

void App::update() {}
void App::updateModel() {}
void App::updateCamera() {}
void App::updateSkybox() {}
void App::CreateViewAndPerspective()
{
    // Use DirectXMath to create view and perspective matrices.
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(5.0f, 5.0f, 2.5f, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eye, at, up);
    DirectX::XMStoreFloat4x4(&view, viewMatrix);

    float aspectRatio = 1280.0f / 720.0f;
    DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, aspectRatio, 0.01f, 100.0f);
    DirectX::XMStoreFloat4x4(&proj, projMatrix);
}

void App::imgui_window_render_camera()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.5f, 0.0f, 1.0f));
    //camera position
    ImGui::Begin("Change viewing position");
    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Position:");
    ImGui::SliderFloat(" Rightward", &forward, -10.0f, 10.0f);
    ImGui::SliderFloat(" Upward", &upward, -10.0f, 10.0f);
    ImGui::SliderFloat(" Forward", &rightward, -10.0f, 10.0f);
    /*ImGui::Text("Rightward value = %f", &forward);
    ImGui::Text("\nUpward value = %f", &upward);
    ImGui::Text("\nForward value = %f", &rightward);*/
    ImGui::End();
    ImGui::PopStyleColor();
}

void App::imgui_window_render_position()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.451f, 0.3098f, 0.5882f, 1.0f));
    //cube position
    ImGui::Begin("Change mesh position");
    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Position:");
    ImGui::SliderFloat(" X", &x, -10.0f, 10.0f);
    ImGui::SliderFloat(" Y", &y, -10.0f, 10.0f);
    ImGui::SliderFloat(" Z", &z, -10.0f, 10.0f);
    ImGui::InputFloat("X", &x);
    ImGui::InputFloat("Y", &y);
    ImGui::InputFloat("Z", &z);
    ImGui::End();
    ImGui::PopStyleColor();
}

void App::imgui_window_render_rotation()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.133f, 0.545f, 0.133f, 1.0f));
    //cube position
    ImGui::Begin("Change mesh rotation around axes:");
    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Rotation:");
    ImGui::SliderFloat(" Rotation X (Roll)", &m_rot_x, -10.0f, 10.0f);
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Rotation:");
    ImGui::SliderFloat(" Rotation Y (Pitch)", &m_rot_y, -10.0f, 10.0f);
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Rotation:");
    ImGui::SliderFloat(" Rotation Z (Yaw)", &m_rot_z, -10.0f, 10.0f);
    ImGui::End();
    ImGui::PopStyleColor();
}

void App::imgui_window_render_scale()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
    //cube scale
    ImGui::Begin("Change mesh scale");
    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(0.13, 0.13, 0.13, 1), "Current Scale:");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13f, 0.13f, 0.13f, 1.0f));
    ImGui::SliderFloat("Scale X", &m_scale_x, 0.0f, 10.0f);
    ImGui::SliderFloat("Scale Y", &m_scale_y, 0.0f, 10.0f);
    ImGui::SliderFloat("Scale Z", &m_scale_z, 0.0f, 10.0f);
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopStyleColor();
}


void App::imgui_start_menu()
{
    deviceContext->clearRenderTargetColor(graphicsEngine.swapChain, 0.13f, 0.13f, 0.5f, 0.75f);
    deviceContext->clearDepthStencilView(depth_stencil_view);
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, depth_stencil_view);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.588f, 0.294f, 0.0f, 0.75f));
    ImGui::SetWindowPos(ImVec2(500, 500));
    ImGui::Begin("MENU");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13f, 0.13f, 0.13f, 1.0f));
    if (ImGui::Button("START")) {
        gameStarted = true;
        showMenu = false;
    }
    if (ImGui::Button("QUIT")) {
        quit_game = true;
    }
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopStyleColor();
    
}
