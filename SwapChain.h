#pragma once

#include <d3d11.h>
#include <memory>

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, ID3D11Device* device);
	SwapChain(ID3D11Device* device);
	IDXGISwapChain* getSwapChain() const { return swap_chain; }
	ID3D11RenderTargetView* getRenderTargetView() const { return renderTargetView; }
	void createRenderTargetView();
	void setFullscreen(bool full_screen, unsigned int width, unsigned int height);
	void resize(unsigned int width, unsigned int height);
	bool present(bool vsync); // presentation
	~SwapChain();
private:
	void reloadBuffers(unsigned int width, unsigned int height);
private:
	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
private:
	friend class DeviceContext;
};

