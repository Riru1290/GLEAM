#include "../../Camera/Camera.h"
#include "../../Resource/ResourceManager.h"
#include "../../Resource/Effect/EffekseerEffect.h"
#include "EffectController.h"


EffectController::EffectController()
{
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;

	efkRenderer_ = EffekseerRendererDX12::Create(
		MainDevice.Get(),
		CommandQueue.Get(),
		2,
		&format,
		1,
		DXGI_FORMAT_D32_FLOAT,
		false,
		10000
	);

	efkManager_ = Effekseer::Manager::Create(10000);
	efkManager_->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

	efkManager_->SetSpriteRenderer(efkRenderer_->CreateSpriteRenderer());
	efkManager_->SetRibbonRenderer(efkRenderer_->CreateRibbonRenderer());
	efkManager_->SetRingRenderer(efkRenderer_->CreateRingRenderer());
	efkManager_->SetTrackRenderer(efkRenderer_->CreateTrackRenderer());
	efkManager_->SetModelRenderer(efkRenderer_->CreateModelRenderer());

	efkManager_->SetTextureLoader(efkRenderer_->CreateTextureLoader());
	efkManager_->SetModelLoader(efkRenderer_->CreateModelLoader());

	efkMemoryPool_ =
		EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer_->GetGraphicsDevice());
	efkCmdList_ = EffekseerRenderer::CreateCommandList(efkRenderer_->GetGraphicsDevice(), efkMemoryPool_);

	efkRenderer_->SetCommandList(efkCmdList_);

	resEffectTable_ =
	{
		{EFFECT_ID::ATMOSPHERE,RES_ID::EFFECT_ATMOSPHERE}
	};
}

EffectController::~EffectController()
{
}

void EffectController::CyncronizeEffekseerCamera()
{
	Effekseer::Matrix44 fkViewMat;
	Effekseer::Matrix44 fkProjMat;

	auto view = MainCamera.GetViewMatrix();
	auto proj = MainCamera.GetProjectionMatrix();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fkViewMat.Values[i][j] = view.r[i].m128_f32[j];
			fkProjMat.Values[i][j] = proj.r[i].m128_f32[j];
		}
	}

	efkRenderer_->SetCameraMatrix(fkViewMat);
	efkRenderer_->SetProjectionMatrix(fkProjMat);
}

void EffectController::Terminate()
{
	efkManager_->StopAllEffects();

	efkManager_->Release();
}

void EffectController::Update()
{
	CyncronizeEffekseerCamera();

	efkManager_->Update();
	efkMemoryPool_->NewFrame();

	EffekseerRendererDX12::BeginCommandList(efkCmdList_, DxCommand.Get());

	efkRenderer_->BeginRendering();
	efkManager_->Draw();
	efkRenderer_->EndRendering();

	EffekseerRendererDX12::EndCommandList(efkCmdList_);
}

Effekseer::Handle EffectController::PlayEffect(EFFECT_ID effctID, Position3D pos, EFFECT_PLAY_TYPE playType, float speed, float scale)
{
	if (!resEffectTable_.contains(effctID))return Effekseer::Handle();

	const EffekseerEffect& effectObj = ResourceMng.GetEffect(resEffectTable_.at(effctID));

	const Effekseer::EffectRef& effect = effectObj.GetEffect();

	Effekseer::Handle efkHandle = efkManager_->Play(effect, pos.x, pos.y, pos.z);

	return efkHandle;
}

void EffectController::StopEffect(Effekseer::Handle handle)
{
	efkManager_->StopEffect(handle);
}
