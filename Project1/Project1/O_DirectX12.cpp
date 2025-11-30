#include "O_DirectX12.h"

O_DirectX12::~O_DirectX12()
{
	if (factory)
	{
		factory->Release();
		factory = nullptr;
	}
	if (adapter)
	{
		adapter->Release();
		adapter = nullptr;
	}
	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
	if (commandQueue)
	{
		commandQueue->Release();
		commandQueue = nullptr;
	}
	if (device)
	{
		device->Release();
		device = nullptr;
	}
}

bool O_DirectX12::Create(int width, int height, O_Window& window)
{
	if (CreateFactory())
	{
		assert(false && "DXGIファクトリーの作成に失敗");
		return true;
	}

	if (CreateAdapter())
	{
		assert(false && "適切なアダプターの取得に失敗");
		return true;
	}

	if (CreateDevice())
	{
		assert(false && "デバイスの生成に失敗");
		return true;
	}

	if (CreateCommandQueue())
	{
		assert(false && "コマンドQの生成に失敗");
		return true;
	}

	if (CreateSwapChain(width, height, window.GetHwnd()))
	{
		assert(false && "スワップチェインの生成に失敗");
		return true;
	}

	return false;
}

// ファクトリーの作成
bool O_DirectX12::CreateFactory()
{
	UINT createFactoryFlags = 0;

#if defined(_DEBUG) // デバッグビルドではデバッグフラグを設定
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) // エラーハンドリング：ファクトリー作成失敗
		return true;

	return false;
}

// アダプターの作成
bool O_DirectX12::CreateAdapter()
{
	// ハードウェアアダプターを順番に検索
	for (UINT adapterIndex = 0;; ++adapterIndex) 
	{
		adapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter)) // アダプターが見つからない場合は終了
			break;

		// アダプター情報を取得
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) // ソフトウェアアダプターをスキップ
		{
			adapter->Release();
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) // DirectX12対応テスト
			return false;  // 適切なアダプターを発見

		adapter->Release();
	}

	return true;
}

// デバイスの作成
bool O_DirectX12::CreateDevice()
{
	// デバイス作成を試行
	HRESULT hr = D3D12CreateDevice(
		adapter,                 // 使用するアダプター
		D3D_FEATURE_LEVEL_11_0,  // 最小機能レベル
		IID_PPV_ARGS(&device)    // 作成されるデバイス
	);

	if (FAILED(hr)) // フォールバック：ソフトウェアアダプターを試行
	{
		hr = D3D12CreateDevice(
			nullptr,  // 規定アダプター
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device));

		if (FAILED(hr))	return true;

		OutputDebugString("Using software adapter (WARP)\n");
	}
	return false;
}

// コマンドQの生成
bool O_DirectX12::CreateCommandQueue()
{
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;		 //直接実行型
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//通常優先度
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;		 //特別グラフ無し
	queueDesc.NodeMask = 0;									 //単一GPU使用

	HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr)) return true;

	// デバッグ用の名前設定（任意だが推奨）
	commandQueue->SetName(L"Main Command Queue");

	return false;
}

// スワップチェインの生成
bool O_DirectX12::CreateSwapChain(int width, int height, HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1* swapChain1;
	HRESULT hr = factory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
	if (FAILED(hr))
	{
		factory->Release();
		return true;
	}

	hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	swapChain->Release();
	if (FAILED(hr)) return true;

	return false;
}