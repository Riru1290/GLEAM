#pragma once
#include "../Component.h"

/// @brief ローカル,ワールド行列
struct LW_MATRIX_DATA
{
	XMMATRIX local;		/// @brief ローカル行列
	XMMATRIX world;		/// @brief ワールド行列
	float alpha = 1.0f;	/// @brief 透明度
	Vector3 padding;
};

class Renderer : public Component
{
public:

	Renderer();
	virtual ~Renderer() = default;

	/// @brief 描画準備
	virtual void RenderBegin() = 0;

	/// @brief 描画終了
	virtual void RenderEnd() = 0;

	/// @brief 描画終了(半透明)
	virtual void RenderEndTrans() = 0;

	/// @brief 影描画(深度バッファ描画)
	virtual void RenderShadow() {}

protected:

	/// @brief 更新処理
	virtual void UpdateRenderer() = 0;

	/// @brief 解放処理
	virtual void ReleaseRenderer() = 0;
	
private:

	/// @brief 更新処理
	void UpdateComponent() {}

	/// @brief 更新処理
	void LateUpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override;

};

