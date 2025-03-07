#include "SceneBase.h"

void SceneBase::Init()
{
	ObjectSetting();
	UISetting();
	CameraSetting();
	LightSetting();
	EventSetting();
	InitScene();
}

void SceneBase::Update()
{
	UpdateScene();
}

void SceneBase::OnCameraUpdate()
{
	OnCameraUpdateScene();
}

void SceneBase::Draw()
{
	DrawScene();
}
