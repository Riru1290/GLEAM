#pragma once
#include "../../Renderer.h"
#include "../../../../../Common/AABB/BoundingSphere.h"

class Mesh;
class Transform;
class Texture;

/// @brief メッシュレンダラー
class MeshRenderer final : public Renderer
{
public:

	[[nodiscard]] MeshRenderer();
	~MeshRenderer();

	/// @brief 面メッシュ作成
	/// @param texture テクスチャ
	void Create2DMesh(const Texture& texture);

	/// @brief 面メッシュ作成
	/// @param size サイズ
	void Create2DMesh(Size2D size);

	/// @brief 描画対象メッシュ設定
	/// @param model 描画対象メッシュ
	void SetMesh(unique_ptr<Mesh> model);

	/// @brief 描画対象モデルトランスフォーム設定
	/// @param transform 描画対象モデルトランスフォーム
	void SetTransform(Transform& transform);

	/// @brief メッシュの透明度をセット
	void SetMeshAlpha(float alpha);

	/// @brief メッシュにローカル行列をセット
	/// @param matrix ローカル行列(位置回転大きさ)
	void SetMeshLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief メッシュにワールド行列をセット
	/// @param matrix ワールド行列(位置回転大きさ)
	void SetMeshWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief トランスフォームを元にワールド・ローカル行列を更新
	/// @param transform トランスフォーム
	void SetMeshTransformMatrixFromTransform(const Transform& transform);

	/// @brief モデル参照
	/// @return モデル
	[[nodiscard]] Mesh& GetMesh()const { return *mesh_; }

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MESH_RENDERER; }

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

	/// @brief 定数バッファ作成
	void CreateConstantBuffer();

	/// @brief ローカル・ワールド行列バッファ作成
	void CreateConstantBufferLocalWorldMatrix();

	/// @brief Transformをもとにローカル・ワールド行列を更新
	void UpdateLocalWorldMatrix();

	/// @brief バウンディングスフィアデータ更新
	/// @param transform 位置情報
	void UpdateBoundingSphere(const Transform& transform);

	/// @brief オブジェクトがカメラビューに含まれているかをBoundingSphereでチェック
	/// @return オブジェクトがカメラビューに含まれているか
	[[nodiscard]] bool IsObjectInCameraFrustum(const BoundingSphere& sphere)const;

	optional<reference_wrapper<Transform>> transform_;	/// @brief 描画対象モデルトランスフォーム

	unique_ptr<Mesh> mesh_;	/// @brief 描画対象メッシュ

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ローカル・ワールド行列バッファ
	int localWorldHeapNo_;								/// @brief ローカル・ワールド行列ディスクリプタヒープ番号
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ローカル・ワールド行列更新用変数

	BoundingSphere boundingSphereOrigin_;       /// @brief バウンディングスフィア
	BoundingSphere boundingSphere_;             /// @brief バウンディングスフィア
};

