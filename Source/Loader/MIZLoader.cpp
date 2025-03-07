#include "MIZLoader.h"

MIZLoader::MIZLoader()
{
}

MIZLoader::~MIZLoader()
{
}

void MIZLoader::Terminate()
{
}

MODEL_DATA MIZLoader::LoadModel(const string& filePath)
{
    MODEL_DATA modelData;

    ifstream ifs(filePath, std::ios::binary);
    
    if (!ifs)
    {
        assert(0 && "ファイルが見つかりません");
    }
    
    // メッシュデータ取得
    size_t meshCount;
    ifs.read(reinterpret_cast<char*>(&meshCount), sizeof(meshCount));

    modelData.meshes.resize(meshCount);

    for (int i = 0; i < static_cast<int>(meshCount); i++)
    {
        // 頂点データ数取得
        size_t vertexCount;
        ifs.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));

        modelData.meshes[i].verticesAnimation.resize(vertexCount);

        // 頂点データ取得
        for (int j = 0; j < static_cast<int>(vertexCount); j++)
        {
            VERTEX_ANIMATION vertex;
            ifs.read(reinterpret_cast<char*>(&vertex), sizeof(VERTEX_ANIMATION));

            modelData.meshes[i].verticesAnimation[j] = vertex;
        }

        // インデックスデータ数取得
        size_t indexCount;
        ifs.read(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));

        modelData.meshes[i].indices.resize(indexCount);

        // インデックスデータ取得
        for (int j = 0; j < static_cast<int>(indexCount); j++)
        {
            unsigned short index;
            ifs.read(reinterpret_cast<char*>(&index), sizeof(unsigned short));

            modelData.meshes[i].indices[j] = index;
        }

        // テクスチャパス取得

        // 拡散反射光
        size_t texturePathLength;
        ifs.read(reinterpret_cast<char*>(&texturePathLength), sizeof(texturePathLength));

        string path(texturePathLength, '\0');
        ifs.read(&path[0], texturePathLength);

        modelData.meshes[i].diffuseTexturePath = path;

        // 鏡面反射光
        ifs.read(reinterpret_cast<char*>(&texturePathLength), sizeof(texturePathLength));

        path.resize(texturePathLength, '\0');
        ifs.read(&path[0], texturePathLength);

        modelData.meshes[i].specularTexturePath = path;

        // ノーマルマップ
        ifs.read(reinterpret_cast<char*>(&texturePathLength), sizeof(texturePathLength));

        path.resize(texturePathLength, '\0');
        ifs.read(&path[0], texturePathLength);

        modelData.meshes[i].normalTexturePath = path;

        // アンビエントオクルージョン
        ifs.read(reinterpret_cast<char*>(&texturePathLength), sizeof(texturePathLength));

        path.resize(texturePathLength, '\0');
        ifs.read(&path[0], texturePathLength);

        modelData.meshes[i].aoTexturePath = path;
    }


    // ボーンデータ取得
    size_t boneCount;
    ifs.read(reinterpret_cast<char*>(&boneCount), sizeof(boneCount));

    modelData.bones.resize(boneCount);

    for (int i = 0; i < static_cast<int>(boneCount); i++)
    {
        // ボーンネーム取得
        size_t boneNameLength;
        ifs.read(reinterpret_cast<char*>(&boneNameLength), sizeof(boneNameLength));

        string boneName(boneNameLength, '\0');
        ifs.read(&boneName[0], boneNameLength);

        modelData.bones[i].boneName = boneName;

        // 子供の数取得
        size_t childrenCount;
        ifs.read(reinterpret_cast<char*>(&childrenCount), sizeof(childrenCount));

        modelData.bones[i].chiledlenBoneIndices.resize(childrenCount);

        // 子供データ取得
        for (int j = 0; j < static_cast<int>(childrenCount); j++)
        {
            int index;
            ifs.read(reinterpret_cast<char*>(&index), sizeof(int));

            modelData.bones[i].chiledlenBoneIndices[j] = index;
        }

        XMMATRIX gMat;
        ifs.read(reinterpret_cast<char*>(&gMat), sizeof(XMMATRIX));
        modelData.bones[i].globalBindInverseMat = gMat;

        XMMATRIX tMat;
        ifs.read(reinterpret_cast<char*>(&tMat), sizeof(XMMATRIX));
        modelData.bones[i].transformLinkMat = tMat;

    }

    // ボーンが設定されていない場合通常頂点とする
    if (boneCount == 0)
    {
        for (int i = 0; i < static_cast<int>(meshCount); i++)
        {
            int vertexNum = static_cast<int>(modelData.meshes[i].verticesAnimation.size());

            modelData.meshes[i].verticesDefault3D.resize(static_cast<size_t>(vertexNum));

            for (int j = 0; j < vertexNum; j++)
            {
                const auto& vA = modelData.meshes[i].verticesAnimation[j];

                auto& vD = modelData.meshes[i].verticesDefault3D[j];
                vD.pos = vA.pos;
                vD.normal = vA.normal;
                vD.uv = vA.uv;
            }

            modelData.meshes[i].verticesAnimation.clear();
        }
    }

    return modelData;
}

ANIMATION_DATA MIZLoader::LoadAnimation(const string& filePath)
{
    ANIMATION_DATA animData;

    ifstream ifs(filePath, std::ios::binary);

    if (!ifs)
    {
        assert(0 && "ファイルが見つかりません");
    }

    // アニメーションネーム取得
    size_t animNameLength;
    ifs.read(reinterpret_cast<char*>(&animNameLength), sizeof(animNameLength));

    string animName(animNameLength, '\0');
    ifs.read(&animName[0], animNameLength);

    animData.animName = animName;

    // 時間取得
    ifs.read(reinterpret_cast<char*>(&animData.start), sizeof(double));
    ifs.read(reinterpret_cast<char*>(&animData.end), sizeof(double));
    ifs.read(reinterpret_cast<char*>(&animData.frameRate), sizeof(double));

    // アニメーション行列取得
    size_t matArrayLength;
    ifs.read(reinterpret_cast<char*>(&matArrayLength), sizeof(matArrayLength));


    for (int i = 0; i < static_cast<int>(matArrayLength); i++)
    {
        // ボーン名取得
        size_t boneNameLength;
        ifs.read(reinterpret_cast<char*>(&boneNameLength), sizeof(boneNameLength));

        string boneName(boneNameLength, '\0');
        ifs.read(&boneName[0], boneNameLength);

        // マトリックス数取得
        size_t matLength;
        ifs.read(reinterpret_cast<char*>(&matLength), sizeof(matLength));

        animData.animBoneMatrix[boneName].resize(matLength);

        for (int j = 0; j < static_cast<int>(matLength); j++)
        {
            // マトリックス取得
            ifs.read(reinterpret_cast<char*>(&animData.animBoneMatrix[boneName][j]), sizeof(XMMATRIX));
        }

    }

    return animData;
}


