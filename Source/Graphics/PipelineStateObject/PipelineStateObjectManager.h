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

/// @brief PSO�Ǘ�
class PipelineStateObjectManager final : public StaticSingleton<PipelineStateObjectManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �ݒ胊�Z�b�g(���t���[���ŏ��ɌĂԂ���)
	void Reset();

	/// @brief �w�肵��PSO���Z�b�g
	/// @param psoID PipelineStateObjectID
	void SetPSO(PSO_ID psoID);

private:

	PipelineStateObjectManager();
	~PipelineStateObjectManager();
	PROOF_OF_STATIC_SINGLETON(PipelineStateObjectManager);

	/// @brief �w�肵��PSO�擾
	/// @param id PSO_ID
	/// @return �w�肳�ꂽPSO
	[[nodiscard]] const PSO& GetPSO(PSO_ID id);

	/// @brief �f�t�H���g��PSO���쐬
	void CreateDefaultPSO();

	unordered_map<PSO_ID, PSO> psoList_;	/// @brief PipelineStateObject���X�g

	PSO_ID currentPSOID_;					/// @brief ���ݓK�p����Ă���PSO
};

