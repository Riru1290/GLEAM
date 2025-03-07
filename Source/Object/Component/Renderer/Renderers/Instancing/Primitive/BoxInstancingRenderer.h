#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Box;

/// @brief ボックスレンダラー(インスタンシング描画)
class BoxInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:

	[[nodiscard]] BoxInstancingRenderer();
	~BoxInstancingRenderer();

	/// @brief 描画インスタンス情報追加
	/// @param center 中心座標
	/// @param r 半径
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	
	/// @brief 描画インスタンス情報追加
	/// @param Pos1 ボックスを構成する二点中の一点の座標
	/// @param Pos2 ボックスを構成する二点中の一点の座標
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void Draw(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag);

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_RENDERER; }

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

	unique_ptr<Box> box_;						/// @brief 描画対象ボックス
};

