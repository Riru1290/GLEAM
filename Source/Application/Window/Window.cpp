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
	// ���b�Z�[�W���ɏ�����I��
	switch (message)
	{
	case WM_SETCURSOR:
		// �N���C�A���g�̈���ł���΁A�ʏ�̃J�[�\����ݒ�
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	case WM_DESTROY:


		PostQuitMessage(0);	// OS�ɑ΂��ďI����`����
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
	// �C���X�^���X�n���h���擾
	hInst_ = GetModuleHandle(0);

	//�[�[�[�[�[�[�[�[�[�[�[
	// ���C���E�B���h�E�쐬
	//�[�[�[�[�[�[�[�[�[�[�[

	// �E�B���h�E�N���X�̒�`
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);					// �\���̂̃T�C�Y
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;		// �E�B���h�E�N���X��
	wc.lpszClassName = windowClassName.c_str();		// �E�B���h�E�N���X��
	wc.hInstance = hInst_;							// �C���X�^���X�n���h��

	wc.hIcon = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_MYICON)); // �傫���A�C�R��
	wc.hIconSm = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_MYICON)); // �������A�C�R��

	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// �E�B���h�E�̍쐬
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

	// �E�B���h�E�̕\��
	ShowWindow(hWnd_, SW_SHOW);

	// �E�B���h�E�̍X�V
	UpdateWindow(hWnd_);

	// ���݂̃E�B���h�E�T�C�Y��ۑ�
	GetWindowRect(hWnd_, &windowRect_);


	displaySize_ = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)),
					 static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };

#ifdef _DEBUG

	SetRestoreWindow();

	isFullScreen_ = false;

	return InitializeImGui();

#else
	// �{�[�_�[���X
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
		// �I�����b�Z�[�W������
		if (msg.message == WM_QUIT) {
			return false;
		}

		// ���b�Z�[�W����
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

	// �C�ӂ�ImGui�֐��̌Ăяo��
	// ���L�ł�"DebugWindow"�Ƃ����^�C�g���̃E�B���h�E��\������
	ImGui::Begin("DebugWindow");

	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

	// �`�揈��
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

// ���z�t���X�N���[�����[�h�ɐ؂�ւ���֐�
void Window::SetVirtualFullScreen() {
	
	// �E�B���h�E�X�^�C����ύX
	SetWindowLong(hWnd_, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	// �v���C�}�����j�^�[�̏����擾
	HMONITOR hMonitor = MonitorFromWindow(hWnd_, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(hMonitor, &mi);

	// �E�B���h�E���v���C�}�����j�^�[�S�̂Ɋg��
	SetWindowPos(hWnd_, NULL, mi.rcMonitor.left, mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top,
		SWP_NOZORDER | SWP_FRAMECHANGED);
}

// ���̃E�B���h�E�ɖ߂��֐�
void Window::SetRestoreWindow() {

	// �E�B���h�E�X�^�C�������ɖ߂�
	SetWindowLong(hWnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	// �E�B���h�E�T�C�Y�����ɖ߂�
	SetWindowPos(hWnd_, NULL, windowRect_.left, windowRect_.top,
		windowRect_.right - windowRect_.left,
		windowRect_.bottom - windowRect_.top,
		SWP_NOZORDER | SWP_FRAMECHANGED);

}
