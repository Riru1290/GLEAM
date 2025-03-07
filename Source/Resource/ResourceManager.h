#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Scene/SceneID.h"
#include "ResourceID.h"
#include "Resource.h"

#define ResourceMng ResourceManager::GetInstance()

class Texture;
class Model;
class Animation;
class Sound;
class EffekseerEffect;

/// @brief ���\�[�X�Ǘ�
class ResourceManager final : public StaticSingleton<ResourceManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �w��̃V�[���ɑΉ��������\�[�X��ǂݍ���&���̑����\�[�X�̉��
	/// @param sceneID 
	void LoadAndRelease(SCENE_ID sceneID);

	/// @brief �e�N�X�`���̒ǉ��ǂݍ���
	/// @param path �e�N�X�`���p�X
	/// @return �e�N�X�`���f�[�^
	const Texture& LoadAdditionalTexture(const string& path);

	/// @brief �e�N�X�`�������\�[�X�}�l�[�W���[�ɕۑ�
	/// @param texture �e�N�X�`��
	/// @return �e�N�X�`���f�[�^
	const Texture& RegisterTextureResource(const string& key, unique_ptr<Texture> texture);

	/// @brief �e�N�X�`���f�[�^�擾
	/// @param id ���\�[�XID
	/// @return �e�N�X�`��
	const Texture& GetTexture(RES_ID id);

	/// @brief �e�N�X�`���f�[�^�擾
	/// @param key �L�[
	/// @return �e�N�X�`��
	const Texture& GetTexture(const string& key);

	/// @brief ���f���f�[�^�擾
	/// @param id ���\�[�XID
	/// @return ���f��
	const Model& GetModel(RES_ID id);

	/// @brief �A�j���[�V�����f�[�^�擾
	/// @param id ���\�[�XID
	/// @return �A�j���[�V����
	const Animation& GetAnimation(RES_ID id);

	/// @brief �T�E���h�f�[�^�擾
	/// @param id ���\�[�XID
	/// @return �T�E���h
	const Sound& GetSound(RES_ID id);

	/// @brief �G�t�F�N�g�f�[�^�擾
	/// @param id ���\�[�XID
	/// @return �G�t�F�N�g
	const EffekseerEffect& GetEffect(RES_ID id);

private:

	/// @brief ���\�[�X���
	enum class RES_TYPE
	{
		IMAGE,
		MODEL,
		ANIMATION,
		SOUND,
		EFFECT,
		MAX
	};

	/// @brief ���\�[�X�\���f�[�^
	struct RES_DATA
	{
		RES_TYPE resType;
		RES_ID resID;
		string path;
		string ext;
	};

	ResourceManager();
	~ResourceManager();
	PROOF_OF_STATIC_SINGLETON(ResourceManager);

	/// @brief ���\�[�X�p�X��JSON�t�@�C������ǂݍ���
	void LoadJsonData();

	/// @brief ���\�[�X��ǂݍ���
	/// @param data ���\�[�X�\���f�[�^
	void Load(RES_DATA data);

	/// @brief ���\�[�X��ǉ��ǂݍ���
	/// @param data ���\�[�X�\���f�[�^
	/// @param key �ǉ����\�[�X���ʃL�[
	void AdditionalLoad(RES_DATA data, const string& key);

	/// @brief �w�胊�\�[�X�����
	/// @param resID ���\�[�XID
	void Release(RES_ID resID);

	/// @brief �w��ǉ����\�[�X�����
	/// @param key �ǉ����\�[�X���ʃL�[
	void Release(const string& key);

	unordered_map<SCENE_ID, vector<RES_DATA>> pathData_;		/// @brief �t�@�C���p�X�f�[�^

	unordered_map<RES_ID, unique_ptr<Resource>> loadedData_;	/// @brief ���[�h�ς݃f�[�^
	unordered_map<string, unique_ptr<Resource>> addLoadedData_;	/// @brief �ǉ����[�h�ς݃f�[�^

	mutex resourceGuard_;										/// @brief ���\�[�X�����ǂݍ��ݗpmutex
};

