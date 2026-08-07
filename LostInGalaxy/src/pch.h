#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <winerror.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXMath.h>
#include <DirectXTex.h>
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "windowscodecs.lib")

#include <iostream>
#include <print>
#include <format>
#include <string>
#include <string_view>
#include <span>
#include <cstdint>
#include <functional>
#include <memory>
#include <chrono>
#include <stdexcept>
#include <bitset>
#include <array>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <system_error>

// ImGUI
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

