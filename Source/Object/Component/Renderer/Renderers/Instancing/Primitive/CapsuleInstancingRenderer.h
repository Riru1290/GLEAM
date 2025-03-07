#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Capsule;

/// @brief カプセルレンダラー(インスタンシング描画)
class CapsuleInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:


	[[nodiscard]] CapsuleInstancingRenderer();
	~CapsuleInstancingRenderer();


	/// @brief 描画インスタンス情報追加
	/// @param pos1 カプセルを形成する二点中の一点の座標
	/// @param pos2 カプセルを形成する二点中の一点の座標
	/// @param radius カプセルの幅
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void Draw(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag);


private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::CAPSULE_RENDERER; }

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

	/// @brief カプセル上半円描画
	void DrawCapsuleTop();

	/// @brief カプセル円柱描画
	void DrawCapsuleMiddle();

	/// @brief カプセル下半円描画
	void DrawCapsuleDown();

	/// @brief 上半円定数バッファ作成
	void CreateConstantBufferSemiCircleTop();

	/// @brief 下半円定数バッファ作成
	void CreateConstantBufferSemiCircleDown();

	unique_ptr<Capsule> capsule_;				/// @brief 描画対象カプセル

	vector<PRIMITIVE_DATA> capTopData_;			/// @brief インスタンスデータ(カプセル上半円用)
	ComPtr<ID3D12Resource> capTopBuffer_;		/// @brief インスタンスデータバッファ(カプセル上半円用)
	int capTopHeapNo_;							/// @brief インスタンスデータディスクリプタヒープ番号(カプセル上半円用)
	PRIMITIVE_DATA* capTopMap_;					/// @brief インスタンスデータ更新用変数(カプセル上半円用)

	vector<PRIMITIVE_DATA> capDownData_;		/// @brief インスタンスデータ(カプセル下半円用)
	ComPtr<ID3D12Resource> capDownBuffer_;		/// @brief インスタンスデータバッファ(カプセル下半円用)
	int capDownHeapNo_;							/// @brief インスタンスデータディスクリプタヒープ番号(カプセル下半円用)
	PRIMITIVE_DATA* capDownMap_;				/// @brief インスタンスデータ更新用変数(カプセル下半円用)
};
