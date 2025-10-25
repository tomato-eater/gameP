#include "DirectX12.h"

#include<dxgi1_4.h>
#include<d3d12.h>

IDXGIFactory4* DirectX12::CreateDXGIFactory()
{
    IDXGIFactory4* factory;
    UINT createFactoryFlags = 0;

#if defined(_DEBUG) // デバッグビルドでデバッグフラグを設定
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

    HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) // エラーハンドリング ファクトリー作成失敗
    {
        OutputDebugString("Failed to create DXGI Factory\n");
        return nullptr;
    }
    return factory;
}

IDXGIAdapter1* DirectX12::GetHardwareAdapter(IDXGIFactory4* factory)
{
    IDXGIAdapter1* adapter;

    for (UINT adapterIndex = 0; ; ++adapterIndex)
    {
        adapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter))
        {
            break;
        }

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            adapter->Release();
            continue;
        }

        if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            return adapter;
        }

        adapter->Release();
    }
    return nullptr;
}

ID3D12Device* DirectX12::CreateD3D12Device(IDXGIAdapter1* adapter)
{
    ID3D12Device* device;

    HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

    if (FAILED(hr))
    {
        hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

        if (FAILED(hr))
        {
            OutputDebugString("Failed to create D3D12 Device\n");
            return nullptr;
        }

        OutputDebugString("Using software adapter (WARP)\n");
        return device;
    }
}

ID3D12CommandQueue* DirectX12::CreateCommandQueue(ID3D12Device* device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    ID3D12CommandQueue* commandQueue;
    HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

    if (FAILED(hr))
    {
        OutputDebugString("Failed to create Command Queue\n");
        return nullptr;
    }

    commandQueue->SetName(L"Main Command Queue");
    return commandQueue;
}

IDXGISwapChain3* DirectX12::CreateSwapChain(IDXGIFactory4* factoty, ID3D12CommandQueue* commandQueue, HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = 1280;
    swapChainDesc.Height = 720;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain1;
    HRESULT hr = factoty->CreateSwapChainForHwnd(
        commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);

    if (FAILED(hr))
    {
        OutputDebugString("Failed to create Swap Chain\n");
        return nullptr;
    }

    IDXGISwapChain3* swapChain;
    hr = swapChain1->Release();

    if (FAILED(hr))
    {
        OutputDebugString("Failed to cast to SwapChain3\n");
        return nullptr;
    }

    return swapChain;
}

void DirectX12::EnableDebugLayer()
{
#if defind(_DEBUG)
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        ID3D12Debug1* debugController1;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController1))))
        {
            debugController1->SetEnableGPUBasedValidation(TRUE)
        }
    }
#endif
}

