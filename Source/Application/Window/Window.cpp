#if defined(_DEBUG)
#include "../../../External/imgui/imgui.h"
#include "../../../External/imgui/imgui_impl_win32.h"
#include "../../../External/imgui/imgui_impl_dx12.h"
#include "../../Utility/Timer/Timer.h"
#endif
#include "../../../resource1.h"
#include "Window.h"

#if defined(_DEBUG)
extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND, UINT, WPARAM, LPARAM
);
#endif

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// メッセージ毎に処理を選択
	switch (message)
	{
	case WM_SETCURSOR:
		// クライアント領域内であれば、通常のカーソルを設定
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	case WM_DESTROY:


		PostQuitMessage(0);	// OSに対して終了を伝える
		break;
	default:
#if defined(_DEBUG)
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
#endif
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

	}

	return 0;
}

bool Window::Create(int width, int height, const std::wstring& titleName, const std::wstring& windowClassName)
{
	// インスタンスハンドル取得
	hInst_ = GetModuleHandle(0);

	//ーーーーーーーーーーー
	// メインウィンドウ作成
	//ーーーーーーーーーーー

	// ウィンドウクラスの定義
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);					// 構造体のサイズ
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;		// ウィンドウクラス名
	wc.lpszClassName = windowClassName.c_str();		// ウィンドウクラス名
	wc.hInstance = hInst_;							// インスタンスハンドル

	wc.hIcon = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_MYICON)); // 大きいアイコン
	wc.hIconSm = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_MYICON)); // 小さいアイコン

	// ウィンドウクラスの登録
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// ウィンドウの作成
	hWnd_ = CreateWindow(
		windowClassName.c_str(),
		titleName.c_str(),
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,
		0,
		0,
		width,
		height,
		nullptr,
		nullptr,
		hInst_,
		this
	);

	if (hWnd_ == nullptr)
	{
		return false;
	}

	// ウィンドウの表示
	ShowWindow(hWnd_, SW_SHOW);

	// ウィンドウの更新
	UpdateWindow(hWnd_);

	// 現在のウィンドウサイズを保存
	GetWindowRect(hWnd_, &windowRect_);


	displaySize_ = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)),
					 static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };

#ifdef _DEBUG

	SetRestoreWindow();

	isFullScreen_ = false;

	return InitializeImGui();

#else
	// ボーダーレス
	SetVirtualFullScreen();

	isFullScreen_ = true;

	return true;

#endif

}

bool Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT) {
			return false;
		}

		// メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::Destroy()
{
#if defined(_DEBUG)
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

#ifdef _DEBUG
bool Window::InitializeImGui()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	MainDevice->CreateDescriptorHeap(
		&desc, IID_PPV_ARGS(ImGuiDescHeap_.ReleaseAndGetAddressOf())
	);

	if (ImGui::CreateContext() == nullptr)
	{
		return false;
	}



	bool result = ImGui_ImplWin32_Init(hWnd_);
	if (!result)
	{
		return false;
	}

	result = ImGui_ImplDX12_Init(
		MainDevice.Get(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		ImGuiDescHeap_.Get(),
		ImGuiDescHeap_->GetCPUDescriptorHandleForHeapStart(),
		ImGuiDescHeap_->GetGPUDescriptorHandleForHeapStart());
	if (!result)
	{
		return false;
	}

	return true;
}

void Window::RenderImGuiBigin()
{

	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 任意のImGui関数の呼び出し
	// 下記では"DebugWindow"というタイトルのウィンドウを表示する
	ImGui::Begin("DebugWindow");

	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

	// 描画処理
	int state = isFullScreen_;
	ImGui::RadioButton("Window:FullScreen", &state, true);
	ImGui::SameLine();
	ImGui::RadioButton("Window:Window", &state, false);
	if (isFullScreen_ != state) {

		if (static_cast<bool>(state) == true) {
			SetVirtualFullScreen();
		}
		else {
			SetRestoreWindow();
		}

		isFullScreen_ = state;
	}

	ImGui::Text("FPS : %3.f", 1.0f / MainTimer.GetDelta());
}

void Window::RenderImGuiEnd()
{
	ImGui::End();

	ImGui::Render();


	ID3D12DescriptorHeap* Heaps[] = { ImGuiDescHeap_.Get() };
	DxCommand->SetDescriptorHeaps(1, Heaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DxCommand.Get());
}

#endif

// 仮想フルスクリーンモードに切り替える関数
void Window::SetVirtualFullScreen() {
	
	// ウィンドウスタイルを変更
	SetWindowLong(hWnd_, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	// プライマリモニターの情報を取得
	HMONITOR hMonitor = MonitorFromWindow(hWnd_, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(hMonitor, &mi);

	// ウィンドウをプライマリモニター全体に拡張
	SetWindowPos(hWnd_, NULL, mi.rcMonitor.left, mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top,
		SWP_NOZORDER | SWP_FRAMECHANGED);
}

// 元のウィンドウに戻す関数
void Window::SetRestoreWindow() {

	// ウィンドウスタイルを元に戻す
	SetWindowLong(hWnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	// ウィンドウサイズを元に戻す
	SetWindowPos(hWnd_, NULL, windowRect_.left, windowRect_.top,
		windowRect_.right - windowRect_.left,
		windowRect_.bottom - windowRect_.top,
		SWP_NOZORDER | SWP_FRAMECHANGED);

}
