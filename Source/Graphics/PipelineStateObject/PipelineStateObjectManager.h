#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "PipelineStateObject.h"
#include "PipelineStateObjectID.h"

#define PSOMng PipelineStateObjectManager::GetInstance()

static bool Is3DPipelineState(PSO_ID psoID)
{
	return (psoID != PSO_ID::DEFAULT_2D &&
		psoID != PSO_ID::SPRITE2D &&
		psoID != PSO_ID::FADE &&
		psoID != PSO_ID::PE_DEFAULT &&
		psoID != PSO_ID::PE_MONOCHROME &&
		psoID != PSO_ID::PE_REVERSE &&
		psoID != PSO_ID::PE_LOCKER &&
		psoID != PSO_ID::SHADOW_DEFAULT &&
		psoID != PSO_ID::SHADOW_ANIMATION &&
		psoID != PSO_ID::SHADOW_2D);
}

/// @brief PSO管理
class PipelineStateObjectManager final : public StaticSingleton<PipelineStateObjectManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 設定リセット(毎フレーム最初に呼ぶこと)
	void Reset();

	/// @brief 指定したPSOをセット
	/// @param psoID PipelineStateObjectID
	void SetPSO(PSO_ID psoID);

private:

	PipelineStateObjectManager();
	~PipelineStateObjectManager();
	PROOF_OF_STATIC_SINGLETON(PipelineStateObjectManager);

	/// @brief 指定したPSO取得
	/// @param id PSO_ID
	/// @return 指定されたPSO
	[[nodiscard]] const PSO& GetPSO(PSO_ID id);

	/// @brief デフォルトのPSOを作成
	void CreateDefaultPSO();

	unordered_map<PSO_ID, PSO> psoList_;	/// @brief PipelineStateObjectリスト

	PSO_ID currentPSOID_;					/// @brief 現在適用されているPSO
};

