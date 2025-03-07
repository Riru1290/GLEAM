#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "Window/Window.h"

#ifdef _DEBUG

// �f�o�b�O��ImGui��L��������
#define ImGuiBegin Application::GetInstance().GetWindow().RenderImGuiBigin()
#define ImGuiEnd Application::GetInstance().GetWindow().RenderImGuiEnd()

#endif

/// @brief �A�v���P�[�V����
class Application final : public StaticSingleton<Application>
{
public:

	/// @brief ���
	void Terminate() override;

	/// @brief ������
	/// @return �������ɐ���������
	bool Initialize();

	/// @brief ���s
	void Execute();

	/// @brief �E�B���h�E�擾
	/// @return �E�B���h�E
	[[nodiscard]] Window& GetWindow() { return window_; };
	
private:

	Application();
	~Application();
	PROOF_OF_STATIC_SINGLETON(Application);

	/// @brief DirectX�I�u�W�F�N�g������
	/// @return ������������������
	bool InitializeDirectX();

	Window window_;		/// @brief �E�B���h�E
};

