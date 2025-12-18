#include "O_ViewRect.h"

void O_ViewRect::Set(int w, int h, O_List& list)
{
	D3D12_VIEWPORT view{};
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width = w;
	view.Height = h;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	list.GetList()->RSSetViewports(1, &view);

	D3D12_RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = w;
	rect.bottom = h;
	list.GetList()->RSSetScissorRects(1, &rect);
}