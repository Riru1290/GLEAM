#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "../../Resource/ResourceID.h"
#include "EffectID.h"

// Effekseer
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>

#ifdef _DEBUG
#pragma comment(lib,"EffekseerRendererDX12d.lib")
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"LLGId.lib")
#else
#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"LLGI.lib")
#endif

#define EffectCon EffectController::GetInstance()

/// @brief エフェクト管理
class EffectController final : public StaticSingleton<EffectController>
{
public:

	

	enum class EFFECT_PLAY_TYPE
	{
		ONCE,	/// @brief 一度再生
		LOOP	/// @brief ループ再生
	};

	/// @brief 解放処理
	void Terminate() override;

	/// @brief エフェクト更新
	void Update();

	/// @brief エフェクト再生
	/// @param effctID エフェクトID
	/// @param pos 座標
	/// @param playType 再生方法
	/// @param speed 再生の速さ
	/// @param scale 大きさ
	/// @return エフェクトハンドル
	Effekseer::Handle PlayEffect(
		EFFECT_ID effctID, Position3D pos,
		EFFECT_PLAY_TYPE playType = EFFECT_PLAY_TYPE::ONCE,
		float speed = 1.0f, float scale = 1.0f);

	void StopEffect(Effekseer::Handle handle);

	/// @brief Effekseerマネージャー取得
	/// @return Effekseerマネージャー
	[[nodiscard]] const Effekseer::ManagerRef& GetEffectManager()const { return efkManager_; }

private:

	EffectController();
	~EffectController();
	PROOF_OF_STATIC_SINGLETON(EffectController);

	/// @brief エフェクシア用カメラをアプリケーション用カメラと同期
	void CyncronizeEffekseerCamera();

	EffekseerRenderer::RendererRef efkRenderer_ = nullptr;
	Effekseer::ManagerRef efkManager_ = nullptr;

	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool_ = nullptr;
	Effekseer::RefPtr<EffekseerRenderer::CommandList> efkCmdList_ = nullptr;

	Effekseer::EffectRef effect_ = nullptr;
	Effekseer::Handle efkHandle_;

	unordered_map<EFFECT_ID, RES_ID> resEffectTable_;
};

