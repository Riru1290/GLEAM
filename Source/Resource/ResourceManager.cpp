#include "../../External/nlohmann/json.hpp"
#include "Texture/Texture.h"
#include "Model/Model.h"
#include "Animation/Animation.h"
#include "Sound/Sound.h"
#include "Effect/EffekseerEffect.h"
#include "ResourceManager.h"


void ResourceManager::Terminate()
{
	// 解放
	for (auto& [id, resource] : loadedData_) {
		Release(id);
	}
	loadedData_.clear();

	for (auto& [key, resource] : addLoadedData_)
	{
		Release(key);
	}
	addLoadedData_.clear();
}

void ResourceManager::LoadAndRelease(SCENE_ID sceneID)
{
	if (!pathData_.contains(sceneID))
	{
		// 解放
		for (auto& [id,resource] : loadedData_) {
			Release(id);
		}
		loadedData_.clear();

		return;
	}

	//for (auto& [key, resource] : addLoadedData_)
	//{
	//	Release(key);
	//}
	//addLoadedData_.clear();

	auto& scenePathData = pathData_.at(sceneID);

	// 解放
	erase_if(loadedData_, [&scenePathData,this](const auto& pair)
		{
			for (auto& resData : scenePathData) {
				if (resData.resID == pair.first) return false;
			}
			Release(pair.first);

			return true;
		});

//#pragma omp parallel for
	for (int i = 0; i < scenePathData.size(); i++)
	{
		Load(scenePathData[i]);
	}

}

const Texture& ResourceManager::LoadAdditionalTexture(const string& path)
{
	size_t last = path.find_last_of('.');
	string key = path.substr(0, last);

	RES_DATA data;
	data.resID = RES_ID::NONE;
	data.resType = RES_TYPE::IMAGE;
	data.path = path;
	data.ext = GetExtension(path);

	// 追加読み込み
	AdditionalLoad(data,key);

	return GetTexture(key);
}

const Texture& ResourceManager::RegisterTextureResource(const string& key, unique_ptr<Texture> texture)
{
	if (!addLoadedData_.contains(key))
	{
		addLoadedData_[key] = move(texture);
	}

	return GetTexture(key);
}



const Texture& ResourceManager::GetTexture(RES_ID id)
{
	if (!loadedData_.contains(id))
	{
		throw runtime_error("Not Load Resource/Texture");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	Texture* texture = dynamic_cast<Texture*>(loadedData_.at(id).get());

	if (texture == nullptr)
	{
		throw runtime_error("Can't Cast To Texture");
	}

	const Texture& ret = *texture;
	 
	return ret;
}

const Texture& ResourceManager::GetTexture(const string& key)
{
	if (!addLoadedData_.contains(key))
	{
		throw runtime_error("Not Load Resource/Texture");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	Texture* texture = dynamic_cast<Texture*>(addLoadedData_.at(key).get());

	if (texture == nullptr)
	{
		throw runtime_error("Can't Cast To Texture");
	}

	const Texture& ret = *texture;

	return ret;
}

const Model& ResourceManager::GetModel(RES_ID id)
{
	if (!loadedData_.contains(id))
	{
		throw runtime_error("Not Load Resource/Model");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	Model* model = dynamic_cast<Model*>(loadedData_.at(id).get());

	if (model == nullptr)
	{
		throw runtime_error("Can't Cast To Model");
	}

	const Model& ret = *model;

 	return ret;
}

const Animation& ResourceManager::GetAnimation(RES_ID id)
{
	if (!loadedData_.contains(id))
	{
		throw runtime_error("Not Load Resource/Animation");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	Animation* animation = dynamic_cast<Animation*>(loadedData_.at(id).get());

	if (animation == nullptr)
	{
		throw runtime_error("Can't Cast To Animation");
	}

	const Animation& ret = *animation;

	return ret;
}

const Sound& ResourceManager::GetSound(RES_ID id)
{
	if (!loadedData_.contains(id))
	{
		throw runtime_error("Not Load Resource/Sound");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	Sound* sound = dynamic_cast<Sound*>(loadedData_.at(id).get());

	if (sound == nullptr)
	{
		throw runtime_error("Can't Cast To Sound");
	}

	const Sound& ret = *sound;

	return ret;
}

const EffekseerEffect& ResourceManager::GetEffect(RES_ID id)
{
	if (!loadedData_.contains(id))
	{
		throw runtime_error("Not Load Resource/Effect");
	}

	// 型変換チェックを行うため,dynamic_castを行う
	EffekseerEffect* effect = dynamic_cast<EffekseerEffect*>(loadedData_.at(id).get());

	if (effect == nullptr)
	{
		throw runtime_error("Can't Cast To Effect");
	}

	const EffekseerEffect& ret = *effect;

	return ret;
}

ResourceManager::ResourceManager()
{
	LoadJsonData();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadJsonData()
{
	using json = nlohmann::json;
	string path = "Resource/Data/Json/";
	// JSONファイルからデータ(パス)読み込み
	// 外部ファイルの読み込み
	ifstream ifs(path + "Resource.json");
	// jsonオブジェクト
	json resourcePathData;
	if (ifs)
	{
		// ファイルストリームからjsonオブジェクトに変換(パース)
		ifs >> resourcePathData;
	}
	else
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	const auto& datas = resourcePathData["Resource"];

	for (auto& data : datas) {
		int id = data["ResourceID"];
		int type = data["ResourceType"];
		string path = data["FilePath"];
		string ext = data["Ext"];
		vector<int> scenes = data["SceneID"];

		RES_DATA resData;
		resData.resID = (RES_ID)(id);
		resData.resType = (RES_TYPE)(type);
		resData.path = path;
		resData.ext = ext;

		for (auto& scene : scenes) {
			pathData_[(SCENE_ID)scene].emplace_back(resData);
		}
	}
}

void ResourceManager::Load(RES_DATA data)
{
	// ロード済みなら返す
	if (loadedData_.contains(data.resID)) return;

	unique_ptr<Resource> tempResouce = nullptr;
	string path = "Resource/";

	switch (data.resType)
	{
	case RES_TYPE::IMAGE:

		path += "Image/";
		path += data.path;
		path += ".";
		path += data.ext;

		tempResouce = make_unique<Texture>(path);

		break;


	case RES_TYPE::MODEL:

		path += "Model/";
		path += data.path;
		path += ".";
		path += data.ext;

		tempResouce = make_unique<Model>(path);

		break;

	case RES_TYPE::ANIMATION:

		path += "Model/";
		path += data.path;
		path += ".";
		path += data.ext;

		tempResouce = make_unique<Animation>(path);

		break;

	case RES_TYPE::SOUND:

		path += "Sound/";
		path += data.path;
		path += ".";
		path += data.ext;

		tempResouce = make_unique<Sound>(path);

		break;

	case RES_TYPE::EFFECT:

		path += "Effect/";
		path += data.path;
		path += ".";
		path += data.ext;

		tempResouce = make_unique<EffekseerEffect>(cp_to_wide(path));

		break;
	}
	
	{
		lock_guard<mutex> lock(resourceGuard_);

		loadedData_.emplace(data.resID, move(tempResouce));
	}
}


void ResourceManager::AdditionalLoad(RES_DATA data, const string& key)
{
	// ロード済みなら返す
	if (addLoadedData_.contains(key)) return;

	unique_ptr<Resource> tempResouce = nullptr;

	switch (data.resType)
	{
	case RES_TYPE::IMAGE:

		tempResouce = make_unique<Texture>(data.path);

		break;


	case RES_TYPE::MODEL:


		tempResouce = make_unique<Model>(data.path);

		break;

	case RES_TYPE::ANIMATION:

		tempResouce = make_unique<Animation>(data.path);

		break;

	case RES_TYPE::SOUND:

		tempResouce = make_unique<Sound>(data.path);

		break;
	}

	addLoadedData_.emplace(key, move(tempResouce));
}

void ResourceManager::Release(RES_ID resID)
{
	if (!loadedData_.contains(resID)) return;

	loadedData_.at(resID)->Release();
}

void ResourceManager::Release(const string& key)
{
	if (!addLoadedData_.contains(key)) return;

	addLoadedData_.at(key)->Release();
}
