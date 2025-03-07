#include "../Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../Common/Transform.h"
#include "../../../Graphics/Light/LightController.h"
#include "FlashLightHolder.h"

namespace
{
	const string FLASHLIGHT_KEY = "FlashlightHolder";
	const string FLASHLIGHT_TIME_KEY = "FlashlightHolder";

	const SPOT_LIGHT SPOT_LIGHT_CONFIG = {
		.pos = {},
		.range = 5000.0f ,
		.direction = {0.0f,0.0f,1.0f},
		.angle = Deg2Radian(25.0f),
		.color = { 1.0f,1.0f,1.0f }
	};
}

FlashLightHolder::FlashLightHolder() : Component(),
light_({}),
goalColor_({}),
nowColor_({})
{
	light_ = SPOT_LIGHT_CONFIG;

	Light.AddSpotLight(light_, FLASHLIGHT_KEY,true);

	goalColor_ = nowColor_ = light_.color;
}

FlashLightHolder::~FlashLightHolder()
{
	Light.DeleteSpotLightInfo(FLASHLIGHT_KEY);
}

void FlashLightHolder::SetGoalSpotLightColor(Vector3 color, float time)
{
	MainTimer.SetTimer(FLASHLIGHT_TIME_KEY, time, true);
	goalColor_ = color;
	nowColor_ = light_.color;
}

void FlashLightHolder::OnCameraUpdateComponent()
{
	if (!transform_.has_value())return;

	relativePos_ = Lerp(startPos_, relativePos_, 0.1f);

	light_.pos = transform_->get().pos;
	light_.pos += transform_->get().qua.PosAxis(relativePos_);
	light_.direction = transform_->get().qua.Mult(
		Quaternion::Euler({Deg2Radian(-2.0f),0.0f,0.0f})).GetForward();

	if (light_.color != goalColor_)
	{
		float rate = 1.0f - MainTimer.GetTime(FLASHLIGHT_TIME_KEY) / MainTimer.GetMaxTime(FLASHLIGHT_TIME_KEY);
		light_.color = Lerp(nowColor_, goalColor_, rate);
	}

	Light.SetSpotLightInfo(light_, FLASHLIGHT_KEY);
}

