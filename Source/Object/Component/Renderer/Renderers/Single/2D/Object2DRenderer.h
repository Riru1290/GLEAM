#pragma once
#include "../../../Renderer.h"

class Mesh;
class Transform;
class Texture;

/// @brief 2Dレンダラー
class Object2DRenderer final : public Renderer
{
public:

	[[nodiscard]] Object2DRenderer();
	~Object2DRenderer();

	/// @brief テクスチャ設定
	/// @param texture テクスチャ
	void SetTexture(const Texture& texture);

	/// @brief メッシュ作成
	/// @param size メッシュサイズ
	void CreateMesh(Size2D size);

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

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] Mesh& GetMesh()const { return *mesh_; }

	/// @brief テクスチャサイズ取得
	/// @return テクスチャサイズ
	[[nodiscard]] const Size2D& GetTextureSize()const { return textureSize_; }

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::OBJECT2D_RENDERER; }


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

	optional<reference_wrapper<Transform>> transform_;	/// @brief 描画対象モデルトランスフォーム

	unique_ptr<Mesh> mesh_;			/// @brief 描画対象メッシュ

	Size2D textureSize_;			/// @brief テクスチャ画像サイズ

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ローカル・ワールド行列バッファ
	int localWorldHeapNo_;								/// @brief ローカル・ワールド行列ディスクリプタヒープ番号
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ローカル・ワールド行列更新用変数
};

