#pragma once
#include <cassert>

#include <d3d12.h>
#include <dxgi1_4.h>

#include "O_Window.h"

/// Factory Adapter Device CommandQueue SwapChain
class O_DirectX12
{
private:
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;
	ID3D12CommandQueue* commandQueue;
	IDXGISwapChain3* swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapDesc;

	bool CreateFactory();
	bool CreateAdapter();
	bool CreateDevice();
	bool CreateCommandQueue();
	bool CreateSwapChain(int width, int height, HWND window);

public:
	O_DirectX12() = default;
	~O_DirectX12();

	[[nodiscard]] bool Create(int width, int height, O_Window& wind);

	ID3D12Device* GetDevice() { return device; }
	ID3D12CommandQueue* GetCommandQ() { return commandQueue; }
	IDXGISwapChain3* GetSwapChain() { return swapChain; }
	DXGI_SWAP_CHAIN_DESC1 GetSwapDesc() { return swapDesc; }
};

