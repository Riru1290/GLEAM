#include "../Common/Transform.h"
#include "../Common/DistanceChecker.h"
#include "../../ObjectManager.h"
#include "../../Props/LightBulb.h"
#include "../Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../../Resource/Model/Model.h"
#include "../../../Common/Mesh/Mesh.h"
#include "../../../Sound/SoundManager.h"
#include "LightBulbPlacer.h"


LightBulbPlacer::LightBulbPlacer(): Component(),
lightBulbNum_(0),
placeLightBulbNum_(0),
lightBulbNumMax_(0)
{
	auto temp = make_shared<LightBulb>("TempLightBulb");
	ObjectMng.AddObject(temp);
	temporaryLightBulbModel_ = temp->GetComponent<ModelRenderer>();
	temporaryLightBulbModel_->get().SetModelAlpha(0.5f);
	
	//for (const auto& mesh : temporaryLightBulbModel_->get().GetModel().GetMeshes())
	//{
	//	mesh.get()->SetTransState(true);
	//}
	temporaryLightBulb_ = temp;


	onCollisionUpdateList_[COLLISION_FASE::SECOND] =
		[this]
		{
			temporaryLightBulb_.lock()->SetTransform(placeLightTransform_);
			temporaryLightBulbModel_->get().LateUpdate();
		};
}

LightBulbPlacer::~LightBulbPlacer()
{
	lightBulbKey_.clear();
}

void LightBulbPlacer::PlaceLightBulb()
{
	if (IsPossible_ != nullptr)
	{
		if (!IsPossible_())return;
	}

	if (placeLightBulbNum_ >= lightBulbNumMax_)
	{
		SoundMng.Play(RES_ID::SOUND_CANTPLACE_LIGHTBULB);
		return;
	}


	SoundMng.Play(RES_ID::SOUND_PLACE_LIGHTBULB);

	string lightKey = "";
	for (const auto& [key, isUsed] : lightBulbKey_)
	{
		if (isUsed)continue;
		lightKey = key;
		break;
	}
	if (lightKey == "")return;

	auto lightBulb = make_shared<LightBulb>(lightKey, lightKey);
	lightBulb->SetTransform(placeLightTransform_);
	lightBulb->SetActiveStateToAllComponents(false);
	auto& checker = lightBulb->GetComponent<DistanceChecker>();

	const auto& enemys = ObjectMng.GetSpecificObjectArray(OBJECT_ID::ENEMY);
	for (const auto& enemy : enemys)
	{
		checker.AddTargetTransform(
			enemy.lock()->GetObjectUniqueKey(),
			enemy.lock()->GetTransform());
	}

	ObjectMng.AddObjectLate(lightBulb);

	lightBulbKey_[lightKey] = true;

	placeLightBulbNum_++;
	placeLightBulbNum_ = min(placeLightBulbNum_, lightBulbNumMax_);

	lightBulbNum_--;
	lightBulbNum_ = max(0, lightBulbNum_);
}

void LightBulbPlacer::PlaceLightBulb(Transform transform)
{
	if (IsPossible_ != nullptr)
	{
		if (!IsPossible_())return;
	}

	if (placeLightBulbNum_ >= lightBulbNumMax_)
	{
		return;
	}

	string lightKey = "";
	for (const auto& [key, isUsed] : lightBulbKey_)
	{
		if (isUsed)continue;
		lightKey = key;
		break;
	}
	if (lightKey == "")return;

	auto lightBulb = make_shared<LightBulb>(lightKey, lightKey);
	lightBulb->SetTransform(transform);
	lightBulb->SetActiveStateToAllComponents(false);
	auto& checker = lightBulb->GetComponent<DistanceChecker>();
	
	const auto& enemys = ObjectMng.GetSpecificObjectArray(OBJECT_ID::ENEMY);
	for (const auto& enemy : enemys)
	{
		checker.AddTargetTransform(
			enemy.lock()->GetObjectUniqueKey(),
			enemy.lock()->GetTransform());
	}


	ObjectMng.AddObjectLate(lightBulb);

	lightBulbKey_[lightKey] = true;

	placeLightBulbNum_++;
	placeLightBulbNum_ = min(placeLightBulbNum_, lightBulbNumMax_);

	lightBulbNum_--;
	lightBulbNum_ = max(0, lightBulbNum_);
}

void LightBulbPlacer::SetLightBulbNumMax(int maxNum)
{
	lightBulbNumMax_ = maxNum;
	lightBulbNum_ = maxNum;

	lightBulbKey_.clear();

	for (int i = 0; i < maxNum; i++)
	{
		string key = "LightBulb" + to_string(i);
		lightBulbKey_[key] = false;
	}
}

void LightBulbPlacer::GetLightBulb()
{
	if (!lightBulbKey_.contains(getLightKey_))return;

	ObjectMng.DeleteObjectLate(getLightKey_);

	lightBulbKey_.at(getLightKey_) = false;

	placeLightBulbNum_--;

	lightBulbNum_++;
}

void LightBulbPlacer::SetIsPossibleCheckFunction(function<bool()> func)
{
	IsPossible_ = func;
}

void LightBulbPlacer::SetTempLightBulbModelTransState(bool isTrans)
{
	temporaryLightBulbModel_->get().SetActive(isTrans);
}

