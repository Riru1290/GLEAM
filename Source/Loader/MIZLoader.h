#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Resource/Model/ModelData.h"

/// @brief MIZ�t�@�C�����[�_�[
class MIZLoader final : public StaticSingleton<MIZLoader>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief MIZ�t�@�C���ǂݍ���
	/// @param filePath �t�@�C���p�X
	/// @return ���f���f�[�^
	[[nodiscard]] MODEL_DATA LoadModel(const string& filePath);

	/// @brief MIZ�t�@�C���ǂݍ���
	/// @param filePath �t�@�C���p�X
	/// @return �A�j���[�V�����f�[�^
	[[nodiscard]] ANIMATION_DATA LoadAnimation(const string& filePath);

private:

	MIZLoader();
	~MIZLoader();
	PROOF_OF_STATIC_SINGLETON(MIZLoader);
};

