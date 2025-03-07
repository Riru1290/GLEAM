#pragma once

/// @brief アプリケーションウィンドウ
class Window
{
public:
	
	/// @brief ウィンドウの作成
	/// @param width 横幅
	/// @param height 縦幅
	/// @param titleName タイトル
	/// @param windowClassName クラス
	/// @return 生成の成否
	virtual bool Create(int width, int height, const std::wstring& titleName, const std::wstring& windowClassName);

	/// @brief ウィンドウメッセージ処理
	/// @return 終了メッセージが来たらfalseが来る
	bool ProcessMessage();

	/// @brief 解放処理
	void Destroy();

	/// @brief ウィンドウハンドル取得
	/// @return ウィンドウハンドル
	[[nodiscard]] HWND GetHWND() const { return hWnd_; }

	/// @brief アプリケーションハンドル取得
	/// @return アプリケーションハンドル
	[[nodiscard]] HINSTANCE GetHInstance() const { return hInst_; }

#ifdef _DEBUG

	/// @brief ImGui初期化
	bool InitializeImGui();

	/// @brief ImGui描画準備
	void RenderImGuiBigin();

	/// @brief ImGui描画終了
	void RenderImGuiEnd();

#endif

	/// @brief 仮想フルスクリーン化
	void SetVirtualFullScreen();

	/// @brief 通常スクリーン化
	void SetRestoreWindow();

	/// @brief ディスプレイ解像度取得
	/// @return ディスプレイ解像度
	[[nodiscard]] const Size2D& GetDisplaySize()const { return displaySize_; }

private:

	HWND hWnd_;					/// @brief ウィンドウハンドル
	HINSTANCE hInst_;			/// @brief アプリケーションInstanceハンドル
	RECT windowRect_;			/// @brief ウィンドウサイズ
	int isFullScreen_ = false;	/// @brief フルスクリーンフラグ
	Vector2 mouseMovePow_;		/// @brief マウス移動量

	Size2D displaySize_;		/// @brief ディスプレイサイズ


#ifdef _DEBUG
	ComPtr<ID3D12DescriptorHeap> ImGuiDescHeap_;	/// @brief ImGui用ディスクリプヒープ
#endif

};

