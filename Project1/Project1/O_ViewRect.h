#pragma once

#include "O_List.h"

class O_ViewRect
{
public:
	O_ViewRect() = default;
	~O_ViewRect() = default;

	void Set(int w, int h, O_List& list);
};

