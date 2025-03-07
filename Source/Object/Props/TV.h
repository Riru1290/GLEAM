#pragma once
#include "../Object.h"

class Material;

/// @brief TV�I�u�W�F�N�g
class TV final : public Object
{
public:

	[[nodiscard]] TV(const string& uniqueKey, RES_ID screenTexture, int xNum, int yNum);
	~TV();

	void ChangeTVChannel(int no);



private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::TV; }

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	struct TV_BUFFER;

	optional<reference_wrapper<Material>> tvScreen_;

	int tvBufferNo_;			/// @brief TVScreen�p�萔�o�b�t�@�Ǘ��ԍ�
	TV_BUFFER* tvMap_;			/// @brief TVScreen�p�萔�X�V�p�ϐ�
};

struct TV::TV_BUFFER
{
	float channel;
	float preChannel;
	float currentTime;
	float changeTime;

	float screenDivideXNum;
	float screenDivideYNum;
	float padding;
	float padding2;
};
