#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Cylinder;

/// @brief シリンダーレンダラー(インスタンシング描画)
class CylinderInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:

	[[nodiscard]] CylinderInstancingRenderer();
	~CylinderInstancingRenderer();

	/// @brief 描画インスタンス情報追加
	/// @param pos シリンダー中心座標
	/// @param height シリンダーの高さ
	/// @param r シリンダー円の半径
	/// @param angle 角度(アグリー)
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void Draw(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag);


private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPHERE_RENDERER; }

	///// @brief 基本定数バッファの数
	//static constexpr int BASIC_CONSTANT_BUFFER_NUM = 3;

	///// @brief レンダラーに最初から登録されている定数バッファの数
	///// @return 基本定数バッファの数
	//[[nodiscard]] int GetBasicConstantBufferNum() const override { return BASIC_CONSTANT_BUFFER_NUM; }

	/// @brief 描画準備
	void RenderBegin()override;

	/// @brief 描画終了
	void RenderEnd()override;

	/// @brief 描画終了(半透明)
	void RenderEndTrans()override {}

	unique_ptr<Cylinder> cylinder_;	/// @brief 描画対象シリンダー
};

