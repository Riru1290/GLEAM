#pragma once
#include "../../Object.h"

class Mesh;

/// @brief ���i�g
struct STRAIGHT_WAVE
{
	Vector2 dir = {1.0f,0.0f};						/// @brief ���g���i�g�̊Ԋu�𒲐��j
	float frequency = 5.0f;							/// @brief �����X�P�[���i�e���͈͂�����������j
	float speed = 0.0f;								/// @brief �g�̃X�s�[�h
	float duration = 0.0f;							/// @brief �p�������鎞��
	float currentTime = 0.0f;						/// @brief �o�ߎ���

	bool isLoop = false;							/// @brief �����ɔ������������邩
};

/// @brief ��g
struct CYCLIC_WAVE
{
	Position3D occurrencePos = { 0.0f,0.0f,0.0f };	/// @brief �����ꏊ
	float frequency = 0.0f;							/// @brief ���g���i�g�̊Ԋu�𒲐��j
	float attenuationScale = 5.0;					/// @brief �����X�P�[���i�e���͈͂�����������j
	float duration = 0.0f;							/// @brief �p�������鎞��
	float currentTime = 0.0f;						/// @brief �o�ߎ���

	bool isLoop = false;							/// @brief �����ɔ������������邩
};

/// @brief �g
class Wave final : public Object
{
public:

	[[nodiscard]] Wave(const string& uniqueKey, const vector<pair<Position3D, Size2D>>& waves);
	~Wave();


	/// @brief ���i�g�ǉ�
	/// @param key �L�[
	/// @param wave �g�̏ڍ�
	void AddStraightWave(STRAIGHT_WAVE wave);

	/// @brief ��g�ǉ�
	/// @param wave �g�̏ڍ�
	void AddCyclicWave(CYCLIC_WAVE wave);

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief �`�揈��
	void DrawObject() override;

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateObject() override;

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateObject() override;

	/// @brief �g���X�V
	void UpdateWaves();

	/// @brief ��g���X�V
	void UpdateCyclicWaves();

	/// @brief ���i�g���X�V
	void UpdateStraightWaves();

	/// @brief �n�C�g�}�b�v�̕ύX��K�p
	void UpdateHeightMap();

	void CreateUploadResource();
	void CreateCopyResource();
	void MapUploadResourceToCopyResource();
	void CopyToGPUResource();

	struct WAVE_BUFFER;

	vector<CYCLIC_WAVE> cyclicWaves_;
	vector<STRAIGHT_WAVE> straightWaves_;

	unique_ptr<Mesh> mesh_;

	int waveBufferNo_;								/// @brief �g�p�萔�o�b�t�@�Ǘ��ԍ�
	WAVE_BUFFER* waveMap_;				/// @brief �g�p�萔�X�V�p�ϐ�


	float count = 0.0f;

	static constexpr int HEIGHT_MAP_WIDTH  = (32);
	static constexpr int HEIGHT_MAP_HEIGHT = (32);

	float heightMap_[HEIGHT_MAP_WIDTH * HEIGHT_MAP_HEIGHT];

	ComPtr<ID3D12Resource> uploadResource_;
	ComPtr<ID3D12Resource> copyResource_;

	int resHeapNo_;

	mutex heightMapGuard_;
};

struct Wave::WAVE_BUFFER
{
	float time;
	float padding;
	float padding2;
	float padding3;
};

