#pragma once
#include "../../../Renderer.h"

class Model;
class Transform;

struct BoundingSphere;

/// @brief アニメーションモデルレンダラー
class AnimationModelRenderer final : public Renderer
{
public:

	[[nodiscard]] AnimationModelRenderer();
	~AnimationModelRenderer();

	/// @brief 描画対象モデル設定
	/// @param model 描画対象モデル
	void SetModel(const Model& model);

	/// @brief 描画対象モデルトランスフォーム設定
	/// @param transform 描画対象モデルトランスフォーム
	void SetTransform(Transform& transform);

	/// @brief モデルの透明度をセット
	void SetModelAlpha(float alpha);

	/// @brief モデルにローカル行列をセット
	/// @param matrix ローカル行列(位置回転大きさ)
	void SetModelLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief モデルにワールド行列をセット
	/// @param matrix ワールド行列(位置回転大きさ)
	void SetModelWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief モデルにボーン行列をセット
	/// @param matrix ボーン行列(アニメーション行列)
	/// @param boneNum ボーン数
	void SetModelBoneMatrix(const XMMATRIX* matrix, int boneNum);

	/// @brief モデルにボーン行列をセット
	/// @param matrix ボーン行列(アニメーション行列)
	/// @param boneNum ボーンインデックス
	void SetModelBoneMatrix(const XMMATRIX& matrix, int boneIndex);

	/// @brief 
	/// @param matrix 
	/// @param boneIndex 
	void SetAnimBoneMatrix(const XMMATRIX& matrix, int boneIndex);

	/// @brief モデル
	/// @param matrix 
	/// @param boneIndex 
	void MultiplyMatrixToModelBoneAndChildren(const XMMATRIX& matrix, int boneIndex);

	/// @brief ローカルボーン行列取得
	/// @param boneIndex ボーンインデックス
	/// @return ローカルボーン行列
	[[nodiscard]] XMMATRIX GetLocalModelBoneMatrix(int boneIndex);

	/// @brief グローバルボーン行列取得
	/// @param boneIndex ボーンインデックス
	/// @return グローバルボーン行列
	[[nodiscard]] XMMATRIX GetGlobalModelBoneMatrix(int boneIndex);


	/// @brief ローカルアニメーション行列取得
	/// @param boneIndex ボーンインデックス
	/// @return ローカルアニメーション行列
	[[nodiscard]] XMMATRIX GetLocalAnimBoneMatrix(int boneIndex);

	/// @brief グローバルアニメーション行列取得
	/// @param boneIndex ボーンインデックス
	/// @return グローバルアニメーション行列
	[[nodiscard]] XMMATRIX GetGlobalAnimBoneMatrix(int boneIndex);

	/// @brief グローバルボーン行列取得
	/// @param boneIndex ボーンインデックス
	/// @return グローバルボーン行列
	[[nodiscard]] XMMATRIX GetGlobalBoneMatrix(int boneIndex);

	/// @brief ボーンフレーム変化行列取得
	/// @param boneIndex ボーンインデックス
	/// @return ボーンフレーム変化行列
	[[nodiscard]] XMMATRIX GetBoneFrameMatrix(int boneIndex);


	/// @brief モデル参照
	/// @return モデル
	[[nodiscard]] const Model& GetModel()const { return model_->get(); }
	
private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ANIMATION_MODEL_RENDERER; }

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

	/// @brief ボーン行列バッファ作成
	void CreateConstantBufferBoneMatrix();

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

	ComPtr<ID3D12Resource> boneConstBuffer_;			/// @brief ボーン行列バッファ
	int boneHeapNo_;									/// @brief ボーン行列ディスクリプタヒープ番号
	XMMATRIX* boneMatrixMap_;							/// @brief ボーン行列更新用変数

	XMMATRIX animMatrix[128];
	XMMATRIX boneFrameMatrix[128];

	vector<BoundingSphere> boundingSphereOrigin_;       /// @brief バウンディングスフィア
	vector<BoundingSphere> boundingSphere_;             /// @brief バウンディングスフィア

};

