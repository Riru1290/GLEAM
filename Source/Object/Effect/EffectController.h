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

/// @brief �G�t�F�N�g�Ǘ�
class EffectController final : public StaticSingleton<EffectController>
{
public:

	

	enum class EFFECT_PLAY_TYPE
	{
		ONCE,	/// @brief ��x�Đ�
		LOOP	/// @brief ���[�v�Đ�
	};

	/// @brief �������
	void Terminate() override;

	/// @brief �G�t�F�N�g�X�V
	void Update();

	/// @brief �G�t�F�N�g�Đ�
	/// @param effctID �G�t�F�N�gID
	/// @param pos ���W
	/// @param playType �Đ����@
	/// @param speed �Đ��̑���
	/// @param scale �傫��
	/// @return �G�t�F�N�g�n���h��
	Effekseer::Handle PlayEffect(
		EFFECT_ID effctID, Position3D pos,
		EFFECT_PLAY_TYPE playType = EFFECT_PLAY_TYPE::ONCE,
		float speed = 1.0f, float scale = 1.0f);

	void StopEffect(Effekseer::Handle handle);

	/// @brief Effekseer�}�l�[�W���[�擾
	/// @return Effekseer�}�l�[�W���[
	[[nodiscard]] const Effekseer::ManagerRef& GetEffectManager()const { return efkManager_; }

private:

	EffectController();
	~EffectController();
	PROOF_OF_STATIC_SINGLETON(EffectController);

	/// @brief �G�t�F�N�V�A�p�J�������A�v���P�[�V�����p�J�����Ɠ���
	void CyncronizeEffekseerCamera();

	EffekseerRenderer::RendererRef efkRenderer_ = nullptr;
	Effekseer::ManagerRef efkManager_ = nullptr;

	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool_ = nullptr;
	Effekseer::RefPtr<EffekseerRenderer::CommandList> efkCmdList_ = nullptr;

	Effekseer::EffectRef effect_ = nullptr;
	Effekseer::Handle efkHandle_;

	unordered_map<EFFECT_ID, RES_ID> resEffectTable_;
};

