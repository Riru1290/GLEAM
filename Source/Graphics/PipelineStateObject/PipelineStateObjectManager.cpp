#include "../../Camera/Camera.h"
#include "../../Camera/Camera2D.h"
#include "../Light/LightController.h"
#include "../Shadow/ShadowRenderer.h"
#include "../RenderManager.h"
#include "PipelineStateObjectCreator.h"
#include "PipelineStateObjectManager.h"

PipelineStateObjectManager::PipelineStateObjectManager() :
    currentPSOID_(PSO_ID::NONE)
{
    CreateDefaultPSO();
}

PipelineStateObjectManager::~PipelineStateObjectManager()
{
}

void PipelineStateObjectManager::Terminate()
{
}

void PipelineStateObjectManager::Reset()
{
    currentPSOID_ = PSO_ID::NONE;
}

void PipelineStateObjectManager::CreateDefaultPSO()
{
    psoList_[PSO_ID::DEFAULT_2D] =
        PSOCreator.CreatePSO(PSO_ID::DEFAULT_2D);
    psoList_[PSO_ID::DEFAULT_3D] =
        PSOCreator.CreatePSO(PSO_ID::DEFAULT_3D);
}

const PSO& PipelineStateObjectManager::GetPSO(PSO_ID id)
{
    if (!psoList_.contains(id))
    {
        psoList_[id] =
            PSOCreator.CreatePSO(id);
    }

    return psoList_.at(id);
}

void PipelineStateObjectManager::SetPSO(PSO_ID psoID)
{
    if (currentPSOID_ == psoID)return;

    auto& pso = GetPSO(psoID);

    // ルートシグネチャセット
    DxCommand->SetGraphicsRootSignature(pso.rootsignature.Get());

    // パイプラインステートを設定
    DxCommand->SetPipelineState(pso.pipelinestate.Get());

    if (Is3DPipelineState(psoID))
    {
        MainCamera.SetBeforeDraw();

        Light.SetBeforeDraw();
    }
    else
    {
        MainCamera2D.SetBeforeDraw();
    }


    currentPSOID_ = psoID;
}
