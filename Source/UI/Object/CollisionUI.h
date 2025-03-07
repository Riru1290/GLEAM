#pragma once
#include "../UIObject.h"
#include "../../Collision/ColliderInfo.h"

class Rect2DCollider;
struct COL_HIT_DATA;

/// @brief �Փ˔�����s��UI
class CollisionUI final : public UIObject
{
public:

	[[nodiscard]] CollisionUI();
	~CollisionUI() = default;

	/// @brief ��`�����Z�b�g
	/// @param rect ��`
	void SetColliderRectInfo(RECT_COL_INFO rect);

	/// @brief �L�����ǂ������Z�b�g
	/// @param isActive �L�����ǂ���
	void SetColliderActiveState(bool isActive);

	/// @brief �Փˌ㏈���֐��ǉ�
	/// @param objectID �ΏۃI�u�W�F�N�g
	/// @param hitData �Փˌ㏈��
	void AddColliderOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit);

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;

	optional<reference_wrapper<Rect2DCollider>> collider_;

	function<void(COL_HIT_DATA)> onHit_;
};

