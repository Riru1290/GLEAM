#pragma once
#include "../../../Renderer.h"
#include "../../../../../../Common/AABB/BoundingSphere.h"

class Model;
class Transform;

/// @brief モデルレンダラー
class ModelRenderer final : public Renderer
{
public:

	[[nodiscard]] ModelRenderer();
	~ModelRenderer();

	/// @brief 描画対象モデル設定
	/// @param model 描画対象モデル
	void SetModel(const Model& model);

	/// @brief 描画対象モデルトランスフォーム設定
	/// @param transform 描画対象モデルトランスフォーム
	void SetTransform(Transform& transform);

	/// @brief モデルにローカル行列をセット
	/// @param matrix ローカル行列(位置回転大きさ)
	void SetModelLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief モデルにワールド行列をセット
	/// @param matrix ワールド行列(位置回転大きさ)
	void SetModelWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief トランスフォームを元にワールド・ローカル行列を更新
	/// @param transform トランスフォーム
	void SetMeshTransformMatrixFromTransform(const Transform& transform);

	/// @brief モデルの透明度をセット
	void SetModelAlpha(float alpha);

	/// @brief モデル参照
	/// @return モデル
	[[nodiscard]] const Model& GetModel()const { return model_->get(); }

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_RENDERER; }

	/// @brief 更新処理
	void UpdateRenderer() override;

	/// @brief 解放処理
	void ReleaseRenderer() override {}

	/// @brief 描画準備
	void RenderBegin()override;

	/// @brief 描画終了
	void RenderEnd()override;

	/// @brief 描画終了(半透明)
	void RenderEndTrans()override;

	/// @brief 影描画(深度バッファ描画)
	void RenderShadow()override;

	/// @brief Transformをもとにローカル・ワールド行列を更新
	void UpdateLocalWorldMatrix();

	/// @brief 定数バッファ作成
	void CreateConstantBuffer();

	/// @brief ローカル・ワールド行列バッファ作成
	void CreateConstantBufferLocalWorldMatrix();

	/// @brief バウンディングスフィアデータ更新
	/// @param transform 位置情報
	void UpdateBoundingSphere(const Transform& transform);

	/// @brief オブジェクトがカメラビューに含まれているかをBoundingSphereでチェック
	/// @return オブジェクトがカメラビューに含まれているか
	[[nodiscard]] bool IsObjectInCameraFrustum(const BoundingSphere& sphere)const;

	optional<reference_wrapper<const Model>> model_;	/// @brief 描画対象モデル
	optional<reference_wrapper<Transform>> transform_;	/// @brief 描画対象モデルトランスフォーム

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ローカル・ワールド行列バッファ
	int localWorldHeapNo_;								/// @brief ローカル・ワールド行列ディスクリプタヒープ番号
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ローカル・ワールド行列更新用変数

	vector<BoundingSphere> boundingSphereOrigin_;       /// @brief バウンディングスフィア
	vector<BoundingSphere> boundingSphere_;             /// @brief バウンディングスフィア
};

