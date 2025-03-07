#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define Factory DXFactory::GetInstance().GetFactory()

/// @brief ファクトリー管理
class DXFactory final : public StaticSingleton<DXFactory>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief Factory作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief Factory取得
	/// @return Factory
	[[nodiscard]] const ComPtr<IDXGIFactory6>& GetFactory();

private:

	DXFactory();
	~DXFactory();
	PROOF_OF_STATIC_SINGLETON(DXFactory);

	ComPtr<IDXGIFactory6> factory_;		/// @brief ファクトリー
};

