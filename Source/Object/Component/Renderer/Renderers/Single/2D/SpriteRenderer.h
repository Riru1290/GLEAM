#pragma once
#include "../../../Renderer.h"

class Mesh;
class Transform;
class Texture;

struct VERTEX_2D;

class SpriteRenderer final : public Renderer
{
public:

	[[nodiscard]] SpriteRenderer();
	~SpriteRenderer();

	/// @brief テクスチャ設定
	/// @param texture テクスチャ
	/// @param xNum X分割数
	/// @param yNum Y分割数
	void SetTexture(const Texture& texture, int xNum, int yNum);

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
	[[nodiscard]] const Mesh& GetMesh()const;

	/// @brief テクスチャ分割サイズを取得
	/// @return テクスチャ分割サイズ
	[[nodiscard]] const Size2D GetTextureSize() const { return texSize_; }

	/// @brief 指定番号の画像を描画する
	/// @param transform 表示位置
	/// @param no 番号
	void RenderSprite(Transform transform, int no);

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPRITE_RENDERER; }


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

	void RecreateVertex();

	optional<reference_wrapper<Transform>> transform_;	/// @brief 描画対象モデルトランスフォーム

	unique_ptr<Mesh> mesh_;								/// @brief 描画対象メッシュ

	vector<pair<Transform,int>> renderInfo_;			/// @brief 描画する画像番号

	Size2D texSize_;
	int divideXNum_;
	int divideYNum_;
	vector<UV> divideUV_;
	UV uvOffset_;

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ローカル・ワールド行列バッファ
	int localWorldHeapNo_;								/// @brief ローカル・ワールド行列ディスクリプタヒープ番号
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ローカル・ワールド行列更新用変数
};

