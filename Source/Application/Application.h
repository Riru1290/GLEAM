#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "Window/Window.h"

#ifdef _DEBUG

// デバッグ中ImGuiを有効化する
#define ImGuiBegin Application::GetInstance().GetWindow().RenderImGuiBigin()
#define ImGuiEnd Application::GetInstance().GetWindow().RenderImGuiEnd()

#endif

/// @brief アプリケーション
class Application final : public StaticSingleton<Application>
{
public:

	/// @brief 解放
	void Terminate() override;

	/// @brief 初期化
	/// @return 初期化に成功したか
	bool Initialize();

	/// @brief 実行
	void Execute();

	/// @brief ウィンドウ取得
	/// @return ウィンドウ
	[[nodiscard]] Window& GetWindow() { return window_; };
	
private:

	Application();
	~Application();
	PROOF_OF_STATIC_SINGLETON(Application);

	/// @brief DirectXオブジェクト初期化
	/// @return 初期化が成功したか
	bool InitializeDirectX();

	Window window_;		/// @brief ウィンドウ
};

