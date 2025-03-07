#if defined(_DEBUG)
#include "../../../../External/imgui/imgui.h"
#include "../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../../../Graphics/Material/Material.h"
#include "../../../Resource/Model/Model.h"
#include "../../../Common/Mesh/Mesh.h"
#include "AnimationController.h"

AnimationController::AnimationController() : Component(),
	currentKey_(""),
	isChangingAnimation_(true)
{
}

void AnimationController::SetAnimationModel(AnimationModelRenderer& modelRenderer)
{
	// ���f�������_���[�Z�b�g
	modelRenderer_ = modelRenderer;

	// ���f���擾
	const Model& model = modelRenderer.GetModel();

	// ���b�V���擾
	const vector<unique_ptr<Mesh>>& meshes = model.GetMeshes();

	// ���b�V����PSO��ANIMATION_MODEL�ɐݒ�
	for (auto& mesh : meshes)
	{
		mesh->GetMaterial().SetPSOID(PSO_ID::ANIMATION_MODEL);
	}
}

void AnimationController::AddAnimation(ANIMATION_DATA& animData, const string& animKey)
{
	ANIM_DATA data = {};
	data.animName = animData.animName;
	data.animBoneMatrix = animData.animBoneMatrix;
	data.start = data.animStartTime = static_cast<float>(animData.start);
	data.end = data.animEndTime = static_cast<float>(animData.end);
	data.total = data.animTotalTime = (static_cast<float>(animData.end) - static_cast<float>(animData.start));
	data.frameRate = static_cast<float>(animData.frameRate);

	animations_[animKey] = data;
}

void AnimationController::ChangeAnimation(const string& animKey)
{
	if (currentKey_ == animKey)return;

	currentKey_ = animKey;

	// �u�����h���v�Z
	float blendRate = 1.0f;
	for (auto& [key, data] : animations_)
	{
		// �A�j���[�V�������K�p����Ă��Ȃ��ꍇ�̓X�L�b�v����
		if (!data.isAnimation || currentKey_ == key)continue;

		blendRate -= data.animBlendRate;
	}

	// �A�j���[�V���������łɍĐ����̏ꍇ�͍Đ����Ԃ����Z�b�g�����Ȃ�
	if (!animations_[currentKey_].isAnimation)
	{
		animations_[currentKey_].animTime = 0.0f;
		animations_[currentKey_].isAnimation = true;
	}
	animations_[currentKey_].animBlendRate = blendRate;

	//animations_[currentKey_].isStop = false;

	isChangingAnimation_ = true;
}

bool AnimationController::IsEndAnimation(const string& animKey)const
{
	if (!animations_.contains(animKey))return false;

	return animations_.at(animKey).animTime == animations_.at(animKey).animEndTime;
}

void AnimationController::SetIsLoop(bool isLoop, const string& animKey)
{
	if (!animations_.contains(animKey))return;

	animations_.at(animKey).isLoop = isLoop;
}

void AnimationController::SetIsStop(bool isStop, const string& animKey)
{
	if (!animations_.contains(animKey))return;

	animations_.at(animKey).isStop = isStop;
}

void AnimationController::SetAnimPlaySpeed(float speed, const string& animKey)
{
	if (!animations_.contains(animKey))return;

	animations_.at(animKey).animSpeed = speed;
}

void AnimationController::SetAnimPlaySpeed(float speed)
{
	for (auto& [key,animData] : animations_)
	{
		animData.animSpeed = speed;
	}
}

void AnimationController::SetOnAnimEndFunction(function<void()> OnAnimEnd, const string& animKey)
{
	if (!animations_.contains(animKey))return;

	animations_.at(animKey).OnAnimEnd = OnAnimEnd;
}

void AnimationController::UpdateComponent()
{
	UpdateAnimationTime();

	UpdateAnimationBlendRate();

	UpdateAnimationMatrix();
}

void AnimationController::DrawComponent()
{
#ifdef _DEBUG
	for (auto& [key, data] : animations_)
	{
		if (!data.isAnimation)continue;

		string text = "AnimRate" + key + ": %f";
		ImGui::Text(text.c_str(), data.animBlendRate);

	}
#endif
}

void AnimationController::ReleaseComponent()
{
}

void AnimationController::UpdateAnimationTime()
{
	float delta = 1.0f / 60.0f;

	for (auto& [key, data] : animations_)
	{
		// �A�j���[�V�������K�p����Ă��Ȃ�
		// �Đ����~�܂��Ă���ꍇ�̓X�L�b�v����
		if (!data.isAnimation || data.isStop)continue;


		// �A�j���[�V�������Ԃ̐i�s
		data.animTime += MainTimer.GetMultDelta(data.animSpeed);
		if (data.animTime > data.animEndTime)
		{
			// ���[�v�Đ����ǂ���
			if (data.isLoop)
			{
				data.animTime = data.animStartTime;
			}
			else
			{
				data.animTime = data.animEndTime;
				data.isStop = true;

				if (data.OnAnimEnd != nullptr)
				{
					data.OnAnimEnd();
				}
			}

		}
	}
}

void AnimationController::UpdateAnimationBlendRate()
{
	if (!isChangingAnimation_ || animations_.size() <= 0)return;

	// �ŗD��A�j���[�V�����̃u�����h��
	float blendRate = 1.0f;

	for (auto& [key, data] : animations_)
	{
		// �A�j���[�V�������K�p����Ă��Ȃ�
		// �ꍇ�̓X�L�b�v����
		if (!data.isAnimation)continue;

		if (key != currentKey_)
		{
			// �D�悳��Ă��Ȃ��A�j���[�V�����̃u�����h���������Ă���
			data.animBlendRate -= MainTimer.GetMultDelta(data.animDetachSpeed);

			if (data.animBlendRate <= 0.0f)
			{
				data.animBlendRate = 0.0f;
				data.isAnimation = false;
			}

			// �ŗD��A�j���[�V�����u�����h����������Ă���
			blendRate -= data.animBlendRate;
		}
	}


	ANIM_DATA& anim = animations_.at(currentKey_);

	// �c�����u�����h�����ŗD��A�j���[�V�����u�����h���Ƃ���
	anim.animBlendRate = blendRate;

	// �u�����h����1.0f�̏ꍇ�͑J�ڂ��I��������
	if (anim.animBlendRate >= 1.0f)
	{
		isChangingAnimation_ = false;
	}
}

void AnimationController::UpdateAnimationMatrix()
{
	// �A�j���[�V�����K�p�惂�f�����ݒ肳��Ă��Ȃ���Ώ�����Ԃ�
	if (!modelRenderer_.has_value())return;

	// ���f�������_���[
	AnimationModelRenderer& modelRenderer = modelRenderer_.value().get();

	// ���f���擾
	const Model& model = modelRenderer.GetModel();

	// �{�[���擾
	const vector<Bone>& bones = model.GetBones();

	int boneNum = static_cast<int>(bones.size());

	// ���f���ƃA�j���[�V�����f�[�^�̃{�[�������r���A�j���[�V����������
#pragma omp parallel for
	for (int index = 0; index < boneNum; index++)
	{
		const auto& bone = bones[index];

		// �u�����h�s��
		optional<XMMATRIX> blendedRotation;
		optional<XMFLOAT3> blendedTranslation;
		optional<XMFLOAT3> blendedScale;

		// �A�j���[�V�������u�����h���Ă���
		for (const auto& [key, animData] : animations_)
		{
			if (!animData.isAnimation)continue;

			// �A�j���[�V�������w��̃{�[���ɑΉ����Ă��邩�ǂ����m�F
			auto itr = animData.animBoneMatrix.find(bone.name);
			if (itr != animData.animBoneMatrix.end())
			{
				// ����̃t���[���̃A�j���[�V�������擾
				float rate = animData.animTime / (1.0f / animData.frameRate);


				// �{�[���̃t���[���p��
				XMMATRIX globalTransform = GetFrameInterpolatedAnimationMatrix(itr->second, rate, animData.isLoop);
				//XMMATRIX globalTransform = animData.animBoneMatrix.at(bone.name)[static_cast<int>(rate)];

				// ��]�����ʐ��`���
				XMVECTOR qua = XMQuaternionRotationMatrix(globalTransform);

				if (blendedRotation.has_value())
				{
					XMVECTOR blendQua = XMQuaternionRotationMatrix(blendedRotation.value());

					blendQua = XMQuaternionSlerp(blendQua, qua, animData.animBlendRate);

					blendedRotation = XMMatrixRotationQuaternion(blendQua);
				}
				else
				{
					blendedRotation = XMMatrixRotationQuaternion(qua);
				}

				// �ʒu����`���
				XMFLOAT3 translation = GetTranslationFromMatrix(globalTransform);
				if (blendedTranslation.has_value())
				{
					XMVECTOR lerp = XMVectorLerp(
						XMLoadFloat3(&blendedTranslation.value()),
						XMLoadFloat3(&translation),
						animData.animBlendRate
					);

					XMFLOAT3 result;
					XMStoreFloat3(&result, lerp);

					blendedTranslation = result;
				}
				else
				{
					blendedTranslation = translation;
				}

				// �X�P�[������`���
				XMFLOAT3 scale = GetScaleFromMatrix(globalTransform);
				if (blendedScale.has_value())
				{
					XMVECTOR lerp = XMVectorLerp(
						XMLoadFloat3(&blendedScale.value()),
						XMLoadFloat3(&scale),
						animData.animBlendRate
					);

					XMFLOAT3 result;
					XMStoreFloat3(&result, lerp);

					blendedScale = result;
				}
				else
				{
					blendedScale = scale;
				}
			}
			else
			{

			}
		}

		// �ŏI�I�ȃu�����h�s����\��
		XMMATRIX blendedTransform = XMMatrixIdentity();
		if (blendedRotation.has_value())
		{
			// �X�P�[��
			XMFLOAT3 scl = blendedScale.value();
			blendedTransform *= XMMatrixScaling(scl.x, scl.y, scl.z);

			// ��]
			blendedTransform *= blendedRotation.value();

			// �ʒu
			XMFLOAT3 pos = blendedTranslation.value();
			blendedTransform *= XMMatrixTranslation(pos.x, pos.y, pos.z);
		}
		else
		{
			blendedTransform = bone.transformLinkMat;
		}

		// �{�[���s��
		XMMATRIX boneMatrix = bone.globalBindInverseMat * blendedTransform * modelRenderer.GetBoneFrameMatrix(index);

		// �Z�b�g
		modelRenderer.SetModelBoneMatrix(boneMatrix, index);
		modelRenderer.SetAnimBoneMatrix(blendedTransform, index);

	}
}

XMMATRIX AnimationController::GetFrameInterpolatedAnimationMatrix(const vector<XMMATRIX>& data, float frame, bool isLoop)
{
	// �t���[���Ԃ̃A�j���[�V�������
	XMMATRIX retMatrix = XMMatrixIdentity();

	// ���������Ə��������ɕ�����
	float integer = 0;
	float fractional = modf(frame, &integer);

	// ���݃t���[���̃{�[���s��
	int nowNo = static_cast<int>(integer);
	if (nowNo >= static_cast<int>(data.size()))
	{
		nowNo = static_cast<int>(data.size()) - 1;
	}
	const XMMATRIX& nowTransform = data[nowNo];


	// 1�t���[����̃{�[���s��
	int newNo = nowNo + 1;
	if (newNo >= static_cast<int>(data.size()))
	{
		if (isLoop) {
			newNo = 0;
		}
		else
		{
			newNo = static_cast<int>(data.size()) - 1;
		}
	}
	const XMMATRIX& newTransform = data[newNo];

	// ���

	// ��]
	XMVECTOR nowQua = XMQuaternionRotationMatrix(nowTransform);
	XMVECTOR newQua = XMQuaternionRotationMatrix(newTransform);
	XMVECTOR blendQua = XMQuaternionSlerp(nowQua, newQua, fractional);

	XMMATRIX blendedRotation = XMMatrixRotationQuaternion(blendQua);

	// �ʒu
	XMFLOAT3 nowTranslation = GetTranslationFromMatrix(nowTransform);
	XMFLOAT3 newTranslation = GetTranslationFromMatrix(newTransform);
	XMVECTOR lerpTrans = XMVectorLerp(
		XMLoadFloat3(&nowTranslation),
		XMLoadFloat3(&newTranslation),
		fractional
	);
	XMFLOAT3 trans;
	XMStoreFloat3(&trans, lerpTrans);

	XMFLOAT3 blendedTranslation = trans;

	// �傫��
	XMFLOAT3 nowScale = GetScaleFromMatrix(nowTransform);
	XMFLOAT3 newScale = GetScaleFromMatrix(newTransform);
	XMVECTOR lerpScale = XMVectorLerp(
		XMLoadFloat3(&nowScale),
		XMLoadFloat3(&newScale),
		fractional
	);
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, lerpScale);

	XMFLOAT3 blendedScale = scale;

	retMatrix = XMMatrixScaling(blendedScale.x, blendedScale.y, blendedScale.z) *
				blendedRotation *
				XMMatrixTranslation(blendedTranslation.x, blendedTranslation.y, blendedTranslation.z);

	return retMatrix;
}
