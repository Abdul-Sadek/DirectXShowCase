#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

GraphicsEngine* GraphicsEngine::graphics_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
}


void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = mesh_layout_byte_code;
	*size = mesh_layout_size;
}

bool GraphicsEngine::createDevices(HWND hwnd, UINT width, UINT height)
{
    // Create the device and device context
    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &device,
        nullptr,
        &deviceContext
    );

    if (FAILED(result)) {
        return false;
    }

    // Create the swap chain
    swapChain = new SwapChain(hwnd, width, height, device);
    if (!swapChain->getSwapChain()) {
        return false;
    }

    // Depth stencil buffer description
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = 1280;
    depthStencilDesc.Height = 720;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    ID3D11Texture2D* depthStencilBuffer;
    device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = depthStencilDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depth_view);

    return true;
}


GraphicsEngine::~GraphicsEngine()
{
    if (swapChain) { delete swapChain; swapChain = nullptr; }
    if (deviceContext) { deviceContext->Release(); deviceContext = nullptr; }
    if (device) { device->Release(); device = nullptr; }
    GraphicsEngine::graphics_engine = nullptr;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::graphics_engine) throw std::exception("Graphics Engine already created");
	GraphicsEngine::graphics_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::graphics_engine) return;
	delete GraphicsEngine::graphics_engine;
}

GraphicsEngine* GraphicsEngine::get()
{
	return graphics_engine;
}