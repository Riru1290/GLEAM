#pragma once

static constexpr int INSTANCE_MAX_NUM = (1000);

struct PRIMITIVE_DATA
{
    XMMATRIX world; // ワールド行列（位置、回転、スケールなど）
    XMFLOAT4 color; // カラー
};
