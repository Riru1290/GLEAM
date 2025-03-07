#pragma once
#include "PrimitiveInstancingData.h"
#include "../../../Renderer.h"

/// @brief インスタンシングレンダラー
class PrimitiveInstancingRenderer : public Renderer
{
public:

	PrimitiveInstancingRenderer();
	virtual ~PrimitiveInstancingRenderer();

protected:

	/// @brief 定数バッファ作成
	void CreateConstantBuffer();

	vector<PRIMITIVE_DATA> primitiveData_;			/// @brief インスタンスデータ

	ComPtr<ID3D12Resource> instanceBuffer_;			/// @brief インスタンスデータバッファ
	int instanceHeapNo_;							/// @brief インスタンスデータディスクリプタヒープ番号
	PRIMITIVE_DATA* instanceMap_;					/// @brief インスタンスデータ更新用変数

private:

	/// @brief 更新処理
	void UpdateRenderer() override {}

	/// @brief 解放処理
	void ReleaseRenderer() override {}

};

