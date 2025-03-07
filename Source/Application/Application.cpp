#ifdef _DEBUG
#include "../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../Graphics/RenderManager.h"
#include "../Input/InputManager.h"
#include "../Collision/CollisionManager.h"
#include "../Scene/SceneManager.h"
#include "../Utility/Timer/Timer.h"
#include "../Camera/Camera.h"
#include "../Camera/Camera2D.h"
#include "../Game/Event/EventManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/ObjectManager.h"
#include "../Resource/ResourceManager.h"
#include "../UI/UIManager.h"
#include "Application.h"

namespace
{
    const wstring APPLICATION_TITLE = L"GLEAM";
    const wstring WINDOW_CLASS_NAME = L"GLEAM_Window";

    const string APPLICATION_CLOSE_INPUT_KEY = "ESCAPE";
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Terminate()
{
    // シングルトンクラス解放処理
    EventMng.Terminate();
    MainCamera.Terminate();
    MainCamera2D.Terminate();
    MainTimer.Terminate();
    InputMng.Terminate();
    ObjectMng.Terminate();
    UIMng.Terminate();
    SceneMng.Terminate();
    RenderMng.Terminate();
    CollisionMng.Terminate();
    SoundMng.Terminate();
    ResourceMng.Terminate();

#ifdef _DEBUG
    DebugRender.Terminate();
#endif

    DXFence::GetInstance().Terminate();

    
    window_.Destroy();

    // CON後処理
    CoUninitialize();
}

bool Application::Initialize()
{
    // COM初期化
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (!window_.Create(SCREEN_WIDTH, SCREEN_HEIGHT, APPLICATION_TITLE, WINDOW_CLASS_NAME))
    {
        assert(0 && "ウィンドウ作成失敗");
        return false;
    }

    InputMng.AddKey(APPLICATION_CLOSE_INPUT_KEY, { {INPUT_TYPE::Keyboard,VK_ESCAPE} });

    return InitializeDirectX();
}

void Application::Execute()
{
    // 命令のクローズ
    DxCommand->Close();

    while (window_.ProcessMessage() && (!InputMng.Pressed(APPLICATION_CLOSE_INPUT_KEY)))
    {
        MainTimer.Run();

        InputMng.Update();

        RenderMng.RenderBegin();

        SceneMng.Execute();

        RenderMng.RenderEnd();

        SoundMng.Update();
    }
}

bool Application::InitializeDirectX()
{
    if (!DXFactory::GetInstance().Create())
    {
        return false;
    }

    if (!DXDevice::GetInstance().Create())
    {
        return false;
    }

    if (!DXCommandAllocator::GetInstance().Create())
    {
        return false;
    }

    if (!DXCommandList::GetInstance().Create())
    {
        return false;
    }

    if (!DXCommandQueue::GetInstance().Create())
    {
        return false;
    }

    if (!DXSwapChain::GetInstance().Create(window_.GetHWND()))
    {
        return false;
    }

    if (!DXFence::GetInstance().Create())
    {
        return false;
    }

    if (!DXAudio::GetInstance().Create())
    {
        return false;
    }

    return true;
}
