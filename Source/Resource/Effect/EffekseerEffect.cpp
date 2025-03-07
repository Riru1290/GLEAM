#include "EffekseerEffect.h"

namespace
{
	const wstring EFFECT_RESOUCE_PATH = L"Resource/Effect/";
}

EffekseerEffect::EffekseerEffect(const wstring& filename)
{
	effect_ = Effekseer::Effect::Create(
		EffectCon.GetEffectManager(),
		(const EFK_CHAR*)filename.c_str(),
		10.0f,
		(const EFK_CHAR*)EFFECT_RESOUCE_PATH.c_str()
	);
}

EffekseerEffect::~EffekseerEffect()
{
}

void EffekseerEffect::Release()
{
}
