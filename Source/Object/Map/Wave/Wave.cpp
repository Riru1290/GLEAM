#include "../../../Graphics/Material/Material.h"
#include "../../../Resource/Texture/Texture.h"
#include "../../../Resource/ResourceManager.h"
#include "../../../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../../../Graphics/RenderManager.h"
#include "../../Component/Renderer/Renderers/Single/MeshRenderer.h"
#include "../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../Component/Collider/Colliders/BoxCollider.h"
#include "../../../Collision/CollisionManager.h"
#include "Wave.h"

namespace
{
    const Position3D WAVE_BLOCK_CENTER_POS = { 0.0f,0.0f,0.0f };
    const Size2D WAVE_BLOCK_SIZE_DEFAULT = { 100.0f,100.0f };
}

Wave::Wave(const string& uniqueKey, const vector<pair<Position3D, Size2D>>& waves): Object(uniqueKey)
{
	MESH_DATA data;

    for (int num = 0;const auto& [center, size] : waves)
    {
        Size2D halfSize = { size.x / 2.0f, size.y / 2.0f };
        XMFLOAT3 centerPos = { center.x,center.y,center.z };

        // 頂点の初期設定
        VERTEX_3D vertices[] = {
            { XMFLOAT3(center.x + -halfSize.x, center.y,center.z + -halfSize.y),
              XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
              XMFLOAT3(1.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 1.0f) },	// 左上

            { XMFLOAT3(center.x + -halfSize.x,  center.y, center.z + halfSize.y),
              XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
              XMFLOAT3(1.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 1.0f) },	// 左下

            { XMFLOAT3(center.x + halfSize.x,  center.y, center.z + -halfSize.y),
              XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f),
              XMFLOAT3(1.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 1.0f) },	// 右上

            { XMFLOAT3(center.x + halfSize.x, center.y, center.z + halfSize.y),
              XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
              XMFLOAT3(1.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 1.0f) },	// 右下
        };


        // 頂点データを設定
        for (const auto& vertex : vertices) {
            data.verticesDefault3D.emplace_back(vertex);
        }

        // インデックスの設定
        uint32_t indices[] = {
            0,1,2,
            2,1,3
        };

        // インデックスデータを設定
        for (auto& index : indices) {
            index += num * 4;
            data.indices.emplace_back(index);
        }

        num++;

        BOX_COL_INFO box;
        box.pos = center + Position3D(0.0f,150.0f,0.0f);
        box.width = 300.0f;
        box.height = 300.0f;
        box.depth = 300.0f;

        auto collider = make_shared<BoxCollider>();
        collider->SetBoxInfo(box);
        collider->SetRigidState(true);
        collider->SetStaticState(true);

        AddComponent(collider);

        CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::WAVE);
    }

    auto mesh = make_unique<MeshDefault3D>(data, PSO_ID::WAVE);

	auto& material = mesh->GetMaterial();
	material.SetNormalMap(ResourceMng.LoadAdditionalTexture("Resource/Image/Wave/wavenormalmap.png"));

    mesh->SetTransState(true);

	AddComponent(RenderMng.CreateRenderer<MeshRenderer>());


    //XMMATRIX world = XMMatrixIdentity();
    //Scale3D wScl = { 1.0f,1.0f, 1.0f };
    //world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
    //Vector3 wRot = {0.0f,0.0f,0.0f};
    //world *= XMMatrixRotationX(wRot.x);
    //world *= XMMatrixRotationY(wRot.y);
    //world *= XMMatrixRotationZ(wRot.z);
    //Position3D wPos = WAVE_BLOCK_CENTER_POS;
    //world *= XMMatrixTranslation(wPos.x, wPos.y, wPos.z);

    //GetComponent<MeshRenderer>().SetMeshWorldTransformMatrix(world);


    waveMap_ =
        mesh->AddConstantBuffer<WAVE_BUFFER>(sizeof(WAVE_BUFFER), 13, waveBufferNo_);

    fill(begin(heightMap_), end(heightMap_), 1.0f);

    CreateUploadResource();
    CreateCopyResource();
    MapUploadResourceToCopyResource();
    CopyToGPUResource();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    mesh->AddShaderResource(copyResource_, 14, srvDesc);

    CYCLIC_WAVE wave = {};
    wave.occurrencePos = WAVE_BLOCK_CENTER_POS + Position3D(0.0f, 0.0f, 0.0f);
    wave.duration = 20.0f;
    wave.frequency = 30.0f;
    wave.attenuationScale = 25.0f;
    wave.isLoop = true;

    for (int i = 0; i < 15; i++)
    {
        Position3D randPos =
        {
            GetRand(WAVE_BLOCK_CENTER_POS.x - (WAVE_BLOCK_SIZE_DEFAULT.x / 2.0f),WAVE_BLOCK_CENTER_POS.x + (WAVE_BLOCK_SIZE_DEFAULT.x / 2.0f)),
            0.0f,
            GetRand(WAVE_BLOCK_CENTER_POS.z - (WAVE_BLOCK_SIZE_DEFAULT.y / 2.0f),WAVE_BLOCK_CENTER_POS.z + (WAVE_BLOCK_SIZE_DEFAULT.y / 2.0f)),
        };

        wave.occurrencePos = randPos;

        wave.frequency = GetRand(10.0f,30.0f);
        wave.attenuationScale = GetRand(10.0f, 20.0f);

        AddCyclicWave(wave);
    }

    for (int i = 0; i < 5; i++)
    {
        Position3D randPos =
        {
            GetRand(WAVE_BLOCK_CENTER_POS.x - (WAVE_BLOCK_SIZE_DEFAULT.x / 2.0f),WAVE_BLOCK_CENTER_POS.x + (WAVE_BLOCK_SIZE_DEFAULT.x / 2.0f)),
            0.0f,
            GetRand(WAVE_BLOCK_CENTER_POS.z - (WAVE_BLOCK_SIZE_DEFAULT.y / 2.0f),WAVE_BLOCK_CENTER_POS.z + (WAVE_BLOCK_SIZE_DEFAULT.y / 2.0f)),
        };


        wave.occurrencePos = randPos;

        wave.frequency = GetRand(20.0f, 40.0f);
        wave.attenuationScale = GetRand(30.0f, 45.0f);

        AddCyclicWave(wave);
    }

    // モデル読み込み
    GetComponent<MeshRenderer>().SetMesh(move(mesh));
}

Wave::~Wave()
{
}


void Wave::AddStraightWave(STRAIGHT_WAVE wave)
{
    straightWaves_.emplace_back(wave);
}

void Wave::AddCyclicWave(CYCLIC_WAVE wave)
{
    cyclicWaves_.emplace_back(wave);
}

void Wave::DrawObject()
{
}

void Wave::UpdateObject()
{
    count += 0.5f;
    waveMap_->time += MainTimer.GetMultDelta(0.05f);

    UpdateWaves();

    UpdateHeightMap();
}

void Wave::LateUpdateObject()
{
}

void Wave::OnCameraUpdateObject()
{
}

void Wave::UpdateWaves()
{
    UpdateCyclicWaves();

    UpdateStraightWaves();
}

void Wave::UpdateCyclicWaves()
{
    for (auto& wave : cyclicWaves_)
    {
        if (wave.currentTime == wave.duration)continue;

        float rate = 1.0f - (wave.currentTime / wave.duration);
        if (wave.isLoop)rate = 1.0f;

        Vector2 centerUV =
        {
           0.5f + (wave.occurrencePos.x - WAVE_BLOCK_CENTER_POS.x) / WAVE_BLOCK_SIZE_DEFAULT.x,
           0.5f + (wave.occurrencePos.z - WAVE_BLOCK_CENTER_POS.z) / WAVE_BLOCK_SIZE_DEFAULT.y,
        };

#pragma omp parallel for
        for (int x = 0; x < HEIGHT_MAP_WIDTH; x++)
        {
#pragma omp parallel for
            for (int y = 0; y < HEIGHT_MAP_HEIGHT; y++)
            {
                // UV座標
                Vector2 uv = { (float)x / ((float)HEIGHT_MAP_WIDTH - 1.0f),(float)y / ((float)HEIGHT_MAP_HEIGHT - 1.0f) };

                // UVを中心化
                Vector2 uvCentered = uv - centerUV;

                // 中心からの距離
                float distance = uvCentered.Length();

                // 周波数（波の間隔を調整）
                float frequency = wave.frequency * rate; // 高周波で密度を上げる

                // 減衰スケール（影響範囲を小さくする）
                float attenuationScale = wave.attenuationScale / rate;

                // 環状波パターンの生成
                float wavePow = (sin((distance - waveMap_->time) * wave.frequency * 2.0f * 
                    static_cast<float>(M_PI)) + 1.0f) / 2.0f;

                wavePow = pow(wavePow, 3.0f);

                // 値を 0 ～ 1 に正規化
                wavePow = (wavePow + 1.0f) * 0.5f;

                // 減衰処理（距離に応じて急激に減少）
                float attenuation = exp(-distance * attenuationScale); // 指数減衰
                wavePow *= attenuation;

                auto& m = heightMap_[x + y * HEIGHT_MAP_WIDTH];
                m += wavePow;

                m = clamp(m, 0.0f, 1.0f);
            }
        }

        if (wave.isLoop)continue;

        wave.currentTime += MainTimer.GetMultDelta(1.0f);
        if (wave.currentTime >= wave.duration)
        {
            wave.currentTime = wave.duration;
        }
    }
}

void Wave::UpdateStraightWaves()
{
    for (auto& wave : straightWaves_)
    {
        float rate = 1.0f - (wave.currentTime / wave.duration);

        if (rate == 0.0f)continue;

#pragma omp parallel for
        for (int x = 0; x < HEIGHT_MAP_WIDTH; x++)
        {
#pragma omp parallel for
            for (int y = 0; y < HEIGHT_MAP_HEIGHT; y++)
            {
                auto& m = heightMap_[x + y * HEIGHT_MAP_WIDTH];


                // UV座標
                Vector2 uv = { (float)x / ((float)HEIGHT_MAP_WIDTH - 1.0f),(float)y / ((float)HEIGHT_MAP_HEIGHT - 1.0f) };


                // 波の方向に基づく座標変換
                float d = Dot(uv, wave.dir);

                // 波の計算
                float wavePow = sin(wave.frequency * d - wave.speed * waveMap_->time);

                // 波の強度をグレースケールで出力
                float intensity = wavePow * 0.5f + 0.5f; // [-1, 1] -> [0, 1] に変換

                m += wavePow;

                m = max(m, 1.0f);
            }
        }

        if (wave.isLoop)continue;

        wave.currentTime += MainTimer.GetMultDelta(1.0f);
        if (wave.currentTime >= wave.duration)
        {
            wave.currentTime = wave.duration;
        }
    }
}

void Wave::UpdateHeightMap()
{
    MapUploadResourceToCopyResource();

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        copyResource_.Get(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_COPY_DEST
    );
    DxCommand->ResourceBarrier(1, &barrier);

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = HEIGHT_MAP_WIDTH;
    textureDesc.Height = HEIGHT_MAP_HEIGHT;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT; // ハイトマップ用フォーマット
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_TEXTURE_COPY_LOCATION destLocation = {};
    destLocation.pResource = copyResource_.Get();
    destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    destLocation.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = uploadResource_.Get();
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    MainDevice->GetCopyableFootprints(&textureDesc, 0, 1, 0, &srcLocation.PlacedFootprint, nullptr, nullptr, nullptr);

    DxCommand->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        copyResource_.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    DxCommand->ResourceBarrier(1, &barrier);

    fill(begin(heightMap_), end(heightMap_), 0.0f);
}

void Wave::CreateUploadResource()
{
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);

    // GetCopyableFootprints を使って適切なサイズを取得
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    UINT64 totalSize = 0;
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = HEIGHT_MAP_WIDTH;
    textureDesc.Height = HEIGHT_MAP_HEIGHT;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    MainDevice->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalSize);

    // ここで totalSize をバッファサイズとして使う
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(totalSize);

    MainDevice->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadResource_)
    );

}

void Wave::CreateCopyResource()
{
    CD3DX12_HEAP_PROPERTIES texHeapProp(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = HEIGHT_MAP_WIDTH;
    textureDesc.Height = HEIGHT_MAP_HEIGHT;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT; // ハイトマップ用フォーマット
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    MainDevice->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&copyResource_)
    );
}

void Wave::MapUploadResourceToCopyResource()
{
    float* resMap = nullptr;
    uploadResource_->Map(0, nullptr, (void**)&resMap);

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    UINT64 totalSize = 0;
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = HEIGHT_MAP_WIDTH;
    textureDesc.Height = HEIGHT_MAP_HEIGHT;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    MainDevice->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalSize);

    UINT rowPitch = footprint.Footprint.RowPitch / sizeof(float);

    for (int y = 0; y < HEIGHT_MAP_HEIGHT; y++)
    {
        memcpy(resMap + y * rowPitch, &heightMap_[y * HEIGHT_MAP_WIDTH], HEIGHT_MAP_WIDTH * sizeof(float));
    }

    uploadResource_->Unmap(0, nullptr);
}

void Wave::CopyToGPUResource()
{
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = HEIGHT_MAP_WIDTH;
    textureDesc.Height = HEIGHT_MAP_HEIGHT;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT; // ハイトマップ用フォーマット
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_TEXTURE_COPY_LOCATION destLocation = {};
    destLocation.pResource = copyResource_.Get();
    destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    destLocation.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = uploadResource_.Get();
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    MainDevice->GetCopyableFootprints(&textureDesc, 0, 1, 0, &srcLocation.PlacedFootprint, nullptr, nullptr, nullptr);

    DxCommand->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        copyResource_.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    DxCommand->ResourceBarrier(1, &barrier);
}
