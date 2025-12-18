#pragma once

#include <cassert>
#include <Windows.h>
#include <string>
using namespace std;

/// <summary>
/// ウィンドウの登録、作成
/// </summary>
class O_Window
{
private:
	HWND hwnd; 

public:
	O_Window() = default;
	~O_Window() = default;

	[[nodiscard]] bool Create(HINSTANCE hInstance, int width, int height, string name);

	HWND GetHwnd() const { return hwnd; }
};

