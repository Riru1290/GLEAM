#include "PipelineStateObjectCreator.h"

namespace
{
    // �V�F�[�_�[�p�X

    // Default2D
    const wstring VS_PATH_DEFAULT2D = L"Resource/Shader/Vertex/Default2D_VS.cso";
    const wstring PS_PATH_DEFAULT2D = L"Resource/Shader/Pixel/Default2D_PS.cso";

    // Default3D
    const wstring VS_PATH_DEFAULT3D = L"Resource/Shader/Vertex/Default3D_VS.cso";
    const wstring PS_PATH_DEFAULT3D = L"Resource/Shader/Pixel/Default3D_PS.cso";

    // Primitive3D
    const wstring VS_PATH_PRIMITIVE3D = L"Resource/Shader/Vertex/Primitive3D_VS.cso";
    const wstring PS_PATH_PRIMITIVE3D = L"Resource/Shader/Pixel/Primitive3D_PS.cso";

    // Animation3D
    const wstring VS_PATH_ANIMATION3D = L"Resource/Shader/Vertex/Animation3D_VS.cso";
    const wstring PS_PATH_ANIMATION3D = L"Resource/Shader/Pixel/Default3D_PS.cso";

    // Sprite2D
    const wstring VS_PATH_SPRITE2D = L"Resource/Shader/Vertex/Sprite2D_VS.cso";
    const wstring PS_PATH_SPRITE2D = L"Resource/Shader/Pixel/Sprite2D_PS.cso";

    // UI3D
    const wstring VS_PATH_UI3D = L"Resource/Shader/Vertex/Default3D_VS.cso";
    const wstring PS_PATH_UI3D = L"Resource/Shader/Pixel/UI3D_PS.cso";

    // LimLight
    const wstring VS_PATH_LIMLIGHT = L"Resource/Shader/Vertex/Default3D_VS.cso";
    const wstring PS_PATH_LIMLIGHT = L"Resource/Shader/Pixel/Limlight_PS.cso";

    // Wave
    const wstring VS_PATH_WAVE = L"Resource/Shader/Vertex/Wave3D_VS.cso";
    const wstring PS_PATH_WAVE = L"Resource/Shader/Pixel/Wave3D_PS.cso";

    // Fade
    const wstring VS_PATH_FADE = L"Resource/Shader/Vertex/Default2D_VS.cso";
    const wstring PS_PATH_FADE = L"Resource/Shader/Pixel/Fade_PS.cso";

    // TVScreen
    const wstring VS_PATH_TVSCREEN = L"Resource/Shader/Vertex/Default3D_VS.cso";
    const wstring PS_PATH_TVSCREEN = L"Resource/Shader/Pixel/TVScreen_PS.cso";

    // TV
    const wstring VS_PATH_TV = L"Resource/Shader/Vertex/Default3D_VS.cso";
    const wstring PS_PATH_TV = L"Resource/Shader/Pixel/TV_PS.cso";

    // �V���h�E
    const wstring VS_PATH_DEFAULT_SHADOW = L"Resource/Shader/Vertex/ShadowDefault_VS.cso";
    const wstring VS_PATH_ANIAMTION_SHADOW = L"Resource/Shader/Vertex/ShadowAnimation_VS.cso";

    const wstring VS_PATH_2D_SHADOW = L"Resource/Shader/Vertex/Default2D_VS.cso";
    const wstring PS_PATH_2D_SHADOW = L"Resource/Shader/Pixel/ShadowMapShader_PS.cso";

    // �|�X�g�G�t�F�N�g

    // �f�t�H���g
    const wstring VS_PATH_PE_DEFAULT = L"Resource/Shader/Vertex/PostEffect_VS.cso";
    const wstring PS_PATH_PE_DEFAULT = L"Resource/Shader/Pixel/DefaultPE_PS.cso";

    // ���m�N��
    const wstring VS_PATH_PE_MONOCHROME = L"Resource/Shader/Vertex/PostEffect_VS.cso";
    const wstring PS_PATH_PE_MONOCHROME = L"Resource/Shader/Pixel/MonochromePE_PS.cso";

    // ���]
    const wstring VS_PATH_PE_REVERSE = L"Resource/Shader/Vertex/PostEffect_VS.cso";
    const wstring PS_PATH_PE_REVERSE = L"Resource/Shader/Pixel/ReversePE_PS.cso";

    // ���b�J�[
    const wstring VS_PATH_PE_LOCKER = L"Resource/Shader/Vertex/PostEffect_VS.cso";
    const wstring PS_PATH_PE_LOCKER = L"Resource/Shader/Pixel/LockerPE_PS.cso";
}

PipelineStateObjectCreator::PipelineStateObjectCreator()
{
    CreatePSO_[PSO_ID::DEFAULT_2D] =
        bind(&PipelineStateObjectCreator::CreatePSODefault2D, this);
    CreatePSO_[PSO_ID::SPRITE2D] =
        bind(&PipelineStateObjectCreator::CreatePSOSprite2D, this);
    CreatePSO_[PSO_ID::DEFAULT_3D] =
        bind(&PipelineStateObjectCreator::CreatePSODefault3D, this);
    CreatePSO_[PSO_ID::PRIMITIVE_3D] =
        bind(&PipelineStateObjectCreator::CreatePSOPrimitive3D, this);
    CreatePSO_[PSO_ID::ANIMATION_MODEL] =
        bind(&PipelineStateObjectCreator::CreatePSOAnimation3D, this);
    CreatePSO_[PSO_ID::PRIMITIVE_3D_INSTANCING] =
        bind(&PipelineStateObjectCreator::CreatePSOPrimitive3DInstancing, this);
    CreatePSO_[PSO_ID::UI_3D] =
        bind(&PipelineStateObjectCreator::CreatePSOUI3D, this);
    CreatePSO_[PSO_ID::LIMLIGHT] =
        bind(&PipelineStateObjectCreator::CreatePSOLimlight, this);
    CreatePSO_[PSO_ID::WAVE] =
        bind(&PipelineStateObjectCreator::CreatePSOWave, this);
    CreatePSO_[PSO_ID::FADE] =
        bind(&PipelineStateObjectCreator::CreatePSOFade, this);
    CreatePSO_[PSO_ID::TVSCREEN] =
        bind(&PipelineStateObjectCreator::CreatePSOTVScreen, this);
    CreatePSO_[PSO_ID::TV] =
        bind(&PipelineStateObjectCreator::CreatePSOTV, this);
    CreatePSO_[PSO_ID::SHADOW_DEFAULT] =
        bind(&PipelineStateObjectCreator::CreatePSOShadowDefault, this);
    CreatePSO_[PSO_ID::SHADOW_ANIMATION] =
        bind(&PipelineStateObjectCreator::CreatePSOShadowAnimation, this);
    CreatePSO_[PSO_ID::SHADOW_2D] =
        bind(&PipelineStateObjectCreator::CreatePSOShadow2D, this);
    CreatePSO_[PSO_ID::PE_DEFAULT] =
        bind(&PipelineStateObjectCreator::CreatePSODefaultPE, this);
    CreatePSO_[PSO_ID::PE_MONOCHROME] =
        bind(&PipelineStateObjectCreator::CreatePSOMonochromePE, this);
    CreatePSO_[PSO_ID::PE_REVERSE] =
        bind(&PipelineStateObjectCreator::CreatePSOReversePE, this);
    CreatePSO_[PSO_ID::PE_LOCKER] =
        bind(&PipelineStateObjectCreator::CreatePSOLockerPE, this);
}

PipelineStateObjectCreator::~PipelineStateObjectCreator()
{
}

void PipelineStateObjectCreator::Terminate()
{
}

PSO PipelineStateObjectCreator::CreatePSO(PSO_ID psoID)
{
    PSO pso = {};
    if (CreatePSO_.contains(psoID))
    {
        pso = CreatePSO_.at(psoID)();
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSODefault3D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Default3D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    //gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_DEFAULT3D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_DEFAULT3D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Default3D");
    }
    
    return pso;
}

PSO PipelineStateObjectCreator::CreatePSODefault2D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[7] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[7] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Default2D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc2D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_DEFAULT2D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_DEFAULT2D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Default2D");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOPrimitive3D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Primitive3D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
       gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_DEFAULT3D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_DEFAULT3D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Primitive3D");
    }


    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOPrimitive3DInstancing()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);


    dRange[8].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);
    rParam[8].InitAsDescriptorTable(1, &dRange[8]);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Primitive3DInstancing");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };


    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_PRIMITIVE3D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_PRIMITIVE3D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Primitive3DInstancing");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOUI3D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_UI3D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    //gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_UI3D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_UI3D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_UI3D");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOAnimation3D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[14] = {};
    dRange[13].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 4);

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[14] = {};
    rParam[13].InitAsDescriptorTable(1, &dRange[13], D3D12_SHADER_VISIBILITY_VERTEX);

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Animation3D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        {
            "INDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT,0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_ANIMATION3D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_ANIMATION3D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_


    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Animation3D");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOSprite2D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[7] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[7] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Default2D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc2D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_SPRITE2D.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_SPRITE2D.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Default2D");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOLimlight()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Limlight");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    //gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_LIMLIGHT.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_LIMLIGHT.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Limlight");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOWave()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[15] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[15] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[6] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);
    dRange[13].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 4); // �g�p�o�b�t�@
    rParam[13].InitAsDescriptorTable(1, &dRange[13], D3D12_SHADER_VISIBILITY_PIXEL);
    dRange[14].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 18); // �g�p�o�b�t�@
    rParam[14].InitAsDescriptorTable(1, &dRange[14], D3D12_SHADER_VISIBILITY_PIXEL);
    sDesc[5] = CD3DX12_STATIC_SAMPLER_DESC(6);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Wave");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_WAVE.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_WAVE.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Wave");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOFade()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[8] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[8] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);
    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Fade");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc2D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_FADE.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_FADE.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Fade");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOTVScreen()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[14] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[14] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);
    dRange[13].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 4); // TVScreen�p�o�b�t�@
    rParam[13].InitAsDescriptorTable(1, &dRange[13], D3D12_SHADER_VISIBILITY_PIXEL);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_TVScreen");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_TVSCREEN.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_TVSCREEN.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_TVScreen");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOTV()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[13] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[13] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_TV");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc3D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_TV.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_TV.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_TV");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOShadowDefault()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[14] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[14] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);
    dRange[13].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 4);
    rParam[13].InitAsDescriptorTable(1, &dRange[13], D3D12_SHADER_VISIBILITY_VERTEX);
    rParam[3].InitAsDescriptorTable(1, &dRange[3], D3D12_SHADER_VISIBILITY_ALL);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_ShadowDefault");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescShadow(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_DEFAULT_SHADOW.c_str(), &vsBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_ShadowDefault");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOShadowAnimation()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[15] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[15] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc3D(dRange, rParam, sDesc);

    dRange[13].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 4);
    rParam[13].InitAsDescriptorTable(1, &dRange[13], D3D12_SHADER_VISIBILITY_VERTEX);
    dRange[14].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 5);
    rParam[14].InitAsDescriptorTable(1, &dRange[14], D3D12_SHADER_VISIBILITY_VERTEX);
    rParam[3].InitAsDescriptorTable(1, &dRange[3], D3D12_SHADER_VISIBILITY_ALL);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);

    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_ShadowAnimation");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        {
            "INDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT,0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescShadow(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));

    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_ANIAMTION_SHADOW.c_str(), &vsBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_


    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_ShadowAnimation");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOShadow2D()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[7] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[7] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[5] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);


    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);



    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_Shadow2D");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDesc2D(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_2D_SHADOW.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_2D_SHADOW.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_Shadow2D");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSODefaultPE()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[8] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[8] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[6] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);

    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);
    sDesc[5] = CD3DX12_STATIC_SAMPLER_DESC(5);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);


    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_DefaultPE");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescPostEffect(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_PE_DEFAULT.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_PE_DEFAULT.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_DefaultPE");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOMonochromePE()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[8] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[8] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[6] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);

    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);
    sDesc[5] = CD3DX12_STATIC_SAMPLER_DESC(5);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);


    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_MonochromePE");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescPostEffect(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_PE_MONOCHROME.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_PE_MONOCHROME.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_MonochromePE");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOReversePE()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[8] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[8] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[6] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);

    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);
    sDesc[5] = CD3DX12_STATIC_SAMPLER_DESC(5);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);


    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_ReversePE");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescPostEffect(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_PE_REVERSE.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_PE_REVERSE.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_ReversePE");
    }

    return pso;
}

PSO PipelineStateObjectCreator::CreatePSOLockerPE()
{
    PSO pso = {};

    // ���[�g�V�O�l�`���̐���
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

    // �f�B�X�N���v�^�����W
    CD3DX12_DESCRIPTOR_RANGE dRange[8] = {};

    // ���[�g�p�����[�^�z��
    CD3DX12_ROOT_PARAMETER rParam[8] = {};

    // �T���v���[
    CD3DX12_STATIC_SAMPLER_DESC sDesc[6] = {};

    // �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
    SetDefaultRootSignatureDesc2D(dRange, rParam, sDesc);

    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);
    sDesc[5] = CD3DX12_STATIC_SAMPLER_DESC(5);

    // ���[�g�V�O�l�`��������
    rootSignatureDesc.Init(_countof(rParam), rParam, _countof(sDesc), sDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);


    HRESULT hr = MainDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
        rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(pso.rootsignature.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "���[�g�V�O�l�`���쐬���s_LockerPE");
    }

    // �p�C�v���C���X�e�[�g�̐���
    // ���_���C�A�E�g�̐���
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        {
            "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    SetDefaultGraphicsPipelineStateDescPostEffect(
        gpsDesc, pso.rootsignature, inputElementDescs, _countof(inputElementDescs));


    // �V�F�[�_�[�I�u�W�F�N�g�̐���
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    D3DReadFileToBlob(VS_PATH_PE_LOCKER.c_str(), &vsBlob);
    D3DReadFileToBlob(PS_PATH_PE_LOCKER.c_str(), &psBlob);

    gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                 // ���_�V�F�[�_
    gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                 // �s�N�Z���V�F�[�_

    hr = MainDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pso.pipelinestate.ReleaseAndGetAddressOf()));

    if (hr != S_OK)
    {
        assert(0 && "�p�C�v���C���X�e�[�g�쐬���s_LockerPE");
    }

    return pso;
}

void PipelineStateObjectCreator::SetDefaultRootSignatureDesc2D(CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc)
{
    // �萔�o�b�t�@ (b0 - b1)
    dRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);  // �J�������
    dRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);  // ���[�J�����[���h�s��

    // SRV (t0 - t4)
    dRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);  // �f�B�t���[�Y�}�b�v
    dRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);  // �X�y�L�����}�b�v
    dRange[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);  // �m�[�}���}�b�v
    dRange[5].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);  // �A���r�G���g�I�N���[�W�����}�b�v
    dRange[6].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);  // �A���r�G���g�I�N���[�W�����}�b�v

    // ���[�g�p�����[�^�̐ݒ�
    rParam[0].InitAsDescriptorTable(1, &dRange[0], D3D12_SHADER_VISIBILITY_ALL);
    rParam[1].InitAsDescriptorTable(1, &dRange[1], D3D12_SHADER_VISIBILITY_ALL);
    rParam[2].InitAsDescriptorTable(1, &dRange[2], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[3].InitAsDescriptorTable(1, &dRange[3], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[4].InitAsDescriptorTable(1, &dRange[4], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[5].InitAsDescriptorTable(1, &dRange[5], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[6].InitAsDescriptorTable(1, &dRange[6], D3D12_SHADER_VISIBILITY_PIXEL);

    // �T���v���[ (s0 - s4)
    sDesc[0] = CD3DX12_STATIC_SAMPLER_DESC(0);
    sDesc[1] = CD3DX12_STATIC_SAMPLER_DESC(1);
    sDesc[2] = CD3DX12_STATIC_SAMPLER_DESC(2);
    sDesc[3] = CD3DX12_STATIC_SAMPLER_DESC(3);
    sDesc[4] = CD3DX12_STATIC_SAMPLER_DESC(4);
}


void PipelineStateObjectCreator::SetDefaultRootSignatureDesc3D(CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc)
{
    // �萔�o�b�t�@ (b0 - b3)
    dRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // �r���[�v���W�F�N�V�����s��
    dRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1); // ���[�J�����[���h�s��
    dRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2); // �J�������
    dRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3); // ���C�g�̐�

    // SRV (t0 - t8)
    dRange[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);  // �������C�g(����)
    dRange[5].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);  // �f�B���N�V���i�����C�g
    dRange[6].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);  // �|�C���g���C�g
    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);  // �X�|�b�g���C�g
    dRange[8].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);  // �f�B�t���[�Y�}�b�v
    dRange[9].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);  // �X�y�L�����}�b�v
    dRange[10].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6); // �m�[�}���}�b�v
    dRange[11].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 7); // �A���r�G���g�I�N���[�W�����}�b�v
    dRange[12].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 10, 8); // �V���h�E�}�b�v

    // ���[�g�p�����[�^�̐ݒ�
    rParam[0].InitAsDescriptorTable(1, &dRange[0], D3D12_SHADER_VISIBILITY_VERTEX);
    rParam[1].InitAsDescriptorTable(1, &dRange[1], D3D12_SHADER_VISIBILITY_ALL);
    rParam[2].InitAsDescriptorTable(1, &dRange[2], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[3].InitAsDescriptorTable(1, &dRange[3], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[4].InitAsDescriptorTable(1, &dRange[4], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[5].InitAsDescriptorTable(1, &dRange[5], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[6].InitAsDescriptorTable(1, &dRange[6], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[8].InitAsDescriptorTable(1, &dRange[8], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[9].InitAsDescriptorTable(1, &dRange[9], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[10].InitAsDescriptorTable(1, &dRange[10], D3D12_SHADER_VISIBILITY_ALL);
    rParam[11].InitAsDescriptorTable(1, &dRange[11], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[12].InitAsDescriptorTable(1, &dRange[12], D3D12_SHADER_VISIBILITY_PIXEL);

    // �T���v���[
    sDesc[0] = CD3DX12_STATIC_SAMPLER_DESC(0);
    sDesc[0].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[1] = CD3DX12_STATIC_SAMPLER_DESC(1);
    sDesc[1].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[2] = CD3DX12_STATIC_SAMPLER_DESC(2);
    sDesc[2].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[3] = CD3DX12_STATIC_SAMPLER_DESC(3);
    sDesc[3].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[4] = CD3DX12_STATIC_SAMPLER_DESC(4);
    sDesc[4].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
}

void PipelineStateObjectCreator::SetDefaultRootSignatureDescShadow(CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc)
{
    // �萔�o�b�t�@ (b0 - b3)
    dRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // �r���[�v���W�F�N�V�����s��
    dRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1); // ���[�J�����[���h�s��
    dRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2); // �J�������
    dRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3); // ���C�g�̐�

    // SRV (t0 - t8)
    dRange[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);  // �������C�g(����)
    dRange[5].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);  // �f�B���N�V���i�����C�g
    dRange[6].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);  // �|�C���g���C�g
    dRange[7].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);  // �X�|�b�g���C�g
    dRange[8].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);  // �f�B�t���[�Y�}�b�v
    dRange[9].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);  // �X�y�L�����}�b�v
    dRange[10].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6); // �m�[�}���}�b�v
    dRange[11].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 7); // �A���r�G���g�I�N���[�W�����}�b�v
    dRange[12].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 10, 8); // �V���h�E�}�b�v

    // ���[�g�p�����[�^�̐ݒ�
    rParam[0].InitAsDescriptorTable(1, &dRange[0], D3D12_SHADER_VISIBILITY_VERTEX);
    rParam[1].InitAsDescriptorTable(1, &dRange[1], D3D12_SHADER_VISIBILITY_VERTEX);
    rParam[2].InitAsDescriptorTable(1, &dRange[2], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[3].InitAsDescriptorTable(1, &dRange[3], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[4].InitAsDescriptorTable(1, &dRange[4], D3D12_SHADER_VISIBILITY_ALL);
    rParam[5].InitAsDescriptorTable(1, &dRange[5], D3D12_SHADER_VISIBILITY_ALL);
    rParam[6].InitAsDescriptorTable(1, &dRange[6], D3D12_SHADER_VISIBILITY_ALL);
    rParam[7].InitAsDescriptorTable(1, &dRange[7], D3D12_SHADER_VISIBILITY_ALL);
    rParam[8].InitAsDescriptorTable(1, &dRange[8], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[9].InitAsDescriptorTable(1, &dRange[9], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[10].InitAsDescriptorTable(1, &dRange[10], D3D12_SHADER_VISIBILITY_ALL);
    rParam[11].InitAsDescriptorTable(1, &dRange[11], D3D12_SHADER_VISIBILITY_PIXEL);
    rParam[12].InitAsDescriptorTable(1, &dRange[12], D3D12_SHADER_VISIBILITY_PIXEL);

    // �T���v���[ (s4 - s8)
    sDesc[0] = CD3DX12_STATIC_SAMPLER_DESC(4);
    sDesc[0].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[1] = CD3DX12_STATIC_SAMPLER_DESC(5);
    sDesc[1].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[2] = CD3DX12_STATIC_SAMPLER_DESC(6);
    sDesc[2].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[3] = CD3DX12_STATIC_SAMPLER_DESC(7);
    sDesc[3].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    sDesc[4] = CD3DX12_STATIC_SAMPLER_DESC(8);
    sDesc[4].Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
}

void PipelineStateObjectCreator::SetDefaultGraphicsPipelineStateDesc2D(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum)
{
    // �p�C�v���C���X�e�[�g�𐶐�
    gpsDesc.pRootSignature = rootSignature.Get();
    gpsDesc.InputLayout.pInputElementDescs = inputLayout;               // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.InputLayout.NumElements = inputLayoutNum;                   // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   // ���X�^���C�U�[�X�e�[�g

    // �[�x�X�e���V��
    gpsDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC();           // �[�x�X�e���V���ݒ�̏�����
    gpsDesc.DepthStencilState.DepthEnable = FALSE;                     // �[�x�e�X�g�𖳌���
    gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // �[�x�������݂𖳌���
    gpsDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;                           // �[�x�o�b�t�@���g�p���Ȃ�


    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    gpsDesc.BlendState.RenderTarget[0].BlendEnable = true;
    gpsDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
    gpsDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    gpsDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    gpsDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    gpsDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // �g�|���W�^�C�v
    gpsDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;    // �X�g���b�v���̃J�b�g�ݒ�
    gpsDesc.NumRenderTargets = 1;                                             // �����_�[�^�[�Q�b�g��
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // �����_�[�^�[�Q�b�g�t�H�[�}�b�g

    gpsDesc.SampleDesc.Count = 1;                                             // �}���`�T���v�����O�̐ݒ�
    gpsDesc.SampleDesc.Quality = 0;
    gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;                           // �T���v���}�X�N�̐ݒ�
}


void PipelineStateObjectCreator::SetDefaultGraphicsPipelineStateDesc3D(
    D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, 
    ComPtr<ID3D12RootSignature> rootSignature, 
    D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum)
{

    // �p�C�v���C���X�e�[�g�𐶐�
    gpsDesc.pRootSignature = rootSignature.Get();
    gpsDesc.InputLayout.pInputElementDescs = inputLayout;               // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.InputLayout.NumElements = inputLayoutNum;                   // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   // ���X�^���C�U�[�X�e�[�g
    gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    //gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
    gpsDesc.RasterizerState.FrontCounterClockwise = FALSE;


    // �[�x�X�e���V��
    gpsDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    gpsDesc.BlendState.RenderTarget[0].BlendEnable = true;
    gpsDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
    gpsDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    gpsDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    gpsDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    gpsDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    
    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // �g�|���W�^�C�v
    gpsDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;    // �X�g���b�v���̃J�b�g�ݒ�
    gpsDesc.NumRenderTargets = 1;                                             // �����_�[�^�[�Q�b�g��
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // �����_�[�^�[�Q�b�g�t�H�[�}�b�g
    
    gpsDesc.SampleDesc.Count = 1;                                             // �}���`�T���v�����O�̐ݒ�
    gpsDesc.SampleDesc.Quality = 0;
    gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;                           // �T���v���}�X�N�̐ݒ�
}

void PipelineStateObjectCreator::SetDefaultGraphicsPipelineStateDescShadow(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum)
{
    // �p�C�v���C���X�e�[�g�𐶐�
    gpsDesc.pRootSignature = rootSignature.Get();
    gpsDesc.InputLayout.pInputElementDescs = inputLayout;               // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.InputLayout.NumElements = inputLayoutNum;                   // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   // ���X�^���C�U�[�X�e�[�g
    gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    //gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
    gpsDesc.RasterizerState.FrontCounterClockwise = FALSE;


    // �[�x�X�e���V��
    gpsDesc.DepthStencilState.DepthEnable = true;
    gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

    gpsDesc.DepthStencilState.StencilEnable = false;
    gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    gpsDesc.BlendState.RenderTarget[0].BlendEnable = true;
    gpsDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
    gpsDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    gpsDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    gpsDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    gpsDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    gpsDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // �g�|���W�^�C�v
    gpsDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;    // �X�g���b�v���̃J�b�g�ݒ�


    gpsDesc.NumRenderTargets = 1;
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;

    gpsDesc.SampleDesc.Count = 1;                                             // �}���`�T���v�����O�̐ݒ�
    gpsDesc.SampleDesc.Quality = 0;
    gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;                           // �T���v���}�X�N�̐ݒ�



    ///////
 /*   gpsDesc.InputLayout.NumElements = inputLayoutNum;
    gpsDesc.InputLayout.pInputElementDescs = inputLayout;
    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;*/


    ////���X�^���C�U
    //gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//�J�����O
    //gpsDesc.RasterizerState.FrontCounterClockwise = FALSE;
    //gpsDesc.RasterizerState.DepthBias = static_cast<INT>(0.1f);
    //gpsDesc.RasterizerState.SlopeScaledDepthBias = 0.01f;





    //gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    //gpsDesc.BlendState.RenderTarget[0].BlendEnable = true;
    //gpsDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    //gpsDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    //gpsDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    //gpsDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    //gpsDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    //gpsDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;



    ////�f�v�X�X�e���V���X�e�[�g
    //gpsDesc.DepthStencilState.DepthEnable = true;
    //gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    //gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

    //gpsDesc.DepthStencilState.StencilEnable = false;
    //gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    //�S�̓I�Ȑݒ�
    gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    gpsDesc.NodeMask = 0;

}

void PipelineStateObjectCreator::SetDefaultGraphicsPipelineStateDescPostEffect(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum)
{
    // �p�C�v���C���X�e�[�g�𐶐�
    gpsDesc.pRootSignature = rootSignature.Get();
    gpsDesc.InputLayout.pInputElementDescs = inputLayout;               // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.InputLayout.NumElements = inputLayoutNum;                   // ���̓��C�A�E�g�̐ݒ�
    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   // ���X�^���C�U�[�X�e�[�g

    // �[�x�X�e���V��
    gpsDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    //gpsDesc.RasterizerState.DepthClipEnable = true;

    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    gpsDesc.BlendState.RenderTarget[0].BlendEnable = true;
    gpsDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
    gpsDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
    gpsDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
    gpsDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    gpsDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    gpsDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    gpsDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    gpsDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // �g�|���W�^�C�v
    gpsDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;    // �X�g���b�v���̃J�b�g�ݒ�
    gpsDesc.NumRenderTargets = 1;                                             // �����_�[�^�[�Q�b�g��
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // �����_�[�^�[�Q�b�g�t�H�[�}�b�g

    gpsDesc.SampleDesc.Count = 1;                                             // �}���`�T���v�����O�̐ݒ�
    gpsDesc.SampleDesc.Quality = 0;
    gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;                           // �T���v���}�X�N�̐ݒ�
}
