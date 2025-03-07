#pragma once

// 基本
#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <WinUser.h>
#include <wrl/client.h>

// STL

// 入出力系
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// コンテナ系
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

// ユーティリティ系
#include <optional>
#include <variant>
#include <functional>
#include <ranges>

// 並列処理系
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

// デバッグ系
#include <cassert>

// 数学系
#include <numeric>
#include <cmath>
#include <limits>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

// DirectX
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

// システム
#include "System/System.h"

// ユーティリティ
#include "Utility/Utility.h"
