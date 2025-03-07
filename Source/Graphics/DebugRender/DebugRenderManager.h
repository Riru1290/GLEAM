#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "../RendererID.h"

#define DebugRender DebugRenderManager::GetInstance()

/// @brief プリミティブタイプ
enum class PRIMITIVE_TYPE
{
	SPHERE,
	BOX,
	CAPSULE,
	CYLINDER,
};

class PrimitiveInstancingRenderer;

/// @brief プリミティブ型描画クラス
class DebugRenderManager final : public StaticSingleton<DebugRenderManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 球体描画
	/// @param center 中心座標
	/// @param r 半径
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void DrawSphere(Position3D center, float r, unsigned int color, float alpha, int fillFlag);

	/// @brief 描画インスタンス情報追加
	/// @param Pos1 ボックスを構成する二点中の一点の座標
	/// @param Pos2 ボックスを構成する二点中の一点の座標
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void DrawBox(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag);

	/// @brief カプセル描画
	/// @param pos1 カプセルを形成する二点中の一点の座標
	/// @param pos2 カプセルを形成する二点中の一点の座標
	/// @param r カプセルの幅
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void DrawCapsule(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag);

	/// @brief シリンダー描画
	/// @param pos シリンダー中心座標
	/// @param height シリンダーの高さ
	/// @param r シリンダー円の半径
	/// @param angle 角度(アグリー)
	/// @param color 色(0～255)
	/// @param alpha 透明度(0～255)
	/// @param fillFlag ワイヤフレーム表示
	void DrawCylinder(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag);

private:

	DebugRenderManager();
	~DebugRenderManager();
	PROOF_OF_STATIC_SINGLETON(DebugRenderManager);

	unordered_map<PRIMITIVE_TYPE, shared_ptr<PrimitiveInstancingRenderer>> renderers_;		/// @brief デバックレンダラー
};

