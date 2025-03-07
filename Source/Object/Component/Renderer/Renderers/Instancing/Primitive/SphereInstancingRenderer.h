#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Sphere;

/// @brief 球体レンダラー(インスタンシング描画)
class SphereInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:

	[[nodiscard]] SphereInstancingRenderer();
	~SphereInstancingRenderer();

	/// @brief 描画インスタンス情報追加
	/// @param center 中心座標
	/// @param r 半径
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void Draw(Position3D center, float r, unsigned int color, float alpha, int fillFlag);

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPHERE_RENDERER; }

	///// @brief 基本定数バッファの数
	//static constexpr int BASIC_CONSTANT_BUFFER_NUM = 3;

	///// @brief レンダラーに最初から登録されている定数バッファの数
	///// @return 基本定数バッファの数
	//[[nodiscard]] int GetBasicConstantBufferNum()const override { return BASIC_CONSTANT_BUFFER_NUM; };

	/// @brief 描画準備
	void RenderBegin()override;

	/// @brief 描画終了
	void RenderEnd()override;

	/// @brief 描画終了(半透明)
	void RenderEndTrans()override {}

	unique_ptr<Sphere> sphere_;						/// @brief 描画対象スフィア

};

