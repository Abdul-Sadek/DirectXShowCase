#include "SwapChain.h"
#include "RenderSystem.h"
#include "imgui/imgui.h"
#include <exception>

SwapChain::SwapChain(ID3D11Device* device) : device(device)
{
}

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, ID3D11Device* device) : device(device), renderTargetView(nullptr)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	
	HRESULT hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr)) throw std::exception("QueryInterface for IDXGIDevice failed");

	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr)) throw std::exception("GetParent for IDXGIAdapter failed");

	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr)) throw std::exception("GetParent for IDXGIFactory failed");

	hr = dxgiFactory->CreateSwapChain(device, &desc, &swap_chain);
	if (FAILED(hr)) throw std::exception("CreateSwapChain failed");

	createRenderTargetView();
}
void SwapChain::createRenderTargetView() {
	// Get the back buffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr)) {
		throw std::exception("Failed to get back buffer from swap chain");
	}
	// Create the render target view
	hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
	if (FAILED(hr)) {
		pBackBuffer->Release();
		throw std::exception("Failed to create render target view");
	}

	// Release the back buffer as it's no longer needed
	pBackBuffer->Release();
}



void SwapChain::setFullscreen(bool full_screen, unsigned int width, unsigned int height)
{
	resize(width, height);
	swap_chain->SetFullscreenState(full_screen, nullptr);
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
	//resizing the buffers
	swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	//reloading the buffers w.r.t the new size
	reloadBuffers(width, height);
}


bool SwapChain::present(bool vsync)
{
	swap_chain->Present(vsync, NULL);
	return true;
}


SwapChain::~SwapChain()
{
	if (renderTargetView) { renderTargetView->Release(); renderTargetView = nullptr; }
	if (swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
	if (dxgiFactory) { dxgiFactory->Release(); dxgiFactory = nullptr; }
	if (dxgiAdapter) { dxgiAdapter->Release(); dxgiAdapter = nullptr; }
	if (dxgiDevice) { dxgiDevice->Release(); dxgiDevice = nullptr; }
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	
}
