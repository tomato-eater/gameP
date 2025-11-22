#include "ViewPorts.h"

void ViewPort(UINT W,UINT H, ID3D12GraphicsCommandList* commandList)
{
	//設定
	D3D12_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = W; //描画サイズ横
	viewPort.Height = H; //描画サイズ縦
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	commandList->RSSetViewports(1, &viewPort);

	//シザー
	D3D12_RECT scissorRect = {};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = W;
	scissorRect.bottom = H;

	commandList->RSSetScissorRects(1, &scissorRect);
}