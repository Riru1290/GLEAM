#include "UIObject.h"
#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Terminate()
{
	UIObjects_.clear();
}

void UIManager::Execute()
{
	for (const auto& [key, UI] : UIObjects_)
	{
		UI->Update();
	}
}

void UIManager::AddUI(const string& key, shared_ptr<UIObject> UI)
{
	UIObjects_[key] = UI;
}

void UIManager::DeleteUI(const string& key)
{
	if (!UIObjects_.contains(key))return;

	UIObjects_.erase(key);
}

void UIManager::ClearUIs()
{
	UIObjects_.clear();
}


weak_ptr<UIObject> UIManager::GetUI(const string& key)
{
	if (!UIObjects_.contains(key))
	{
		assert(0 && "Žw’è‚ÌU‚ªI“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	}

	return UIObjects_.at(key);
}

