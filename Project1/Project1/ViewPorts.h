#pragma once

#include<d3d12.h>
class ViewPorts
{
public:
	ViewPorts() = default;
	~ViewPorts() {};

	void ViewPort(UINT W, UINT H, ID3D12GraphicsCommandList* commandList);
};

