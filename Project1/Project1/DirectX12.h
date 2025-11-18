#pragma once

#include<dxgi1_4.h>
#include<d3d12.h>

class DirectX12
{
public:
	DirectX12() {}
	~DirectX12() {}

	IDXGIFactory4* CreateDXGIFactory();
	IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory4* factory);
	ID3D12Device* CreateD3D12Device(IDXGIAdapter1* adapter);
	ID3D12CommandQueue* CreateCommandQueue(ID3D12Device* device);
	IDXGISwapChain3* CreateSwapChain(IDXGIFactory4* factoty, ID3D12CommandQueue* commandQueue, HWND hwnd);
	void EnableDebugLayer();
};

