#pragma once
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib") 
#include "../Resource.h"

/// @brief テクスチャデータ
struct TEXTURE_DATA
{
	DXGI_FORMAT format;						/// @brief RGBAフォーマット
	UINT64 width;							/// @brief 幅
	UINT height;							/// @brief 高さ
	UINT16 arraySize;						/// @brief 配列サイズ
	UINT16 mipLevels;						/// @brief ミップ数
	D3D12_RESOURCE_DIMENSION dimension;		/// @brief ディメンション
	void* pixels;							/// @brief 書き込みデータの先頭アドレス
	UINT rowPitch;							/// @brief 1行当たりのデータサイズ
	UINT slicePitch;						/// @brief スライスあたりのデータサイズ
	ScratchImage* scratchImg;				/// @brief 読み込みデータ
};

/// @brief テクスチャ
class Texture final : public Resource
{
public:

	[[nodiscard]]Texture();
	[[nodiscard]]Texture(const string& filePath);
	[[nodiscard]]Texture(XMFLOAT4 color);
	[[nodiscard]]Texture(TEXTURE_DATA data);
	~Texture();

	/// @brief 解放処理
	void Release()override;

	/// @brief テクスチャ分割
	/// @param xNum X分割数
	/// @param yNum Y分割数
	void DivideImage(int xNum, int yNum);

	/// @brief 分割数取得
	/// @return 分割数
	[[nodiscard]] const int& GetDivideNum()const { return divideNum_; }

	/// @brief テクスチャサイズ取得
	/// @return テクスチャサイズ
	[[nodiscard]] const Size2D& GetTextureSize()const { return size_; }

	/// @brief 分割後テクスチャサイズ取得
	/// @return 分割後テクスチャサイズ
	[[nodiscard]] const Size2D& GetDivideSize()const { return divideSize_; }

	/// @brief 分割後UVオフセット取得
	/// @return 分割後UVオフセット
	[[nodiscard]] const UV& GetUVOffset()const { return uvOffset_; }

	/// @brief 分割後UV配列取得
	/// @return 分割後UV配列
	[[nodiscard]] const vector<UV>& GetDivideUV()const { return divideUV_; }


	/// @brief テクスチャデータをセット
	/// @param rootIndex ルートシグネチャインデックス
	void SetTexture(int rootIndex) const;

private:

	/// @brief WIC対応画像読み込み
	void LoadWICTexture(const string& filePath);

	/// @brief DDS画像(ミップマップ)読み込み
	void LoadDDSTexture(const string& filePath);

	/// @brief カラーテクスチャ作成
	void CreateColorTexture(XMFLOAT4 color);

	/// @brief ダミーテクスチャ作成
	void CreateDummyTexture();

	/// @brief テクスチャバッファ作成
	/// @param data テクスチャデータ
	void CreateTextureBuffer(TEXTURE_DATA data);


	ComPtr<ID3D12Resource> texBuffer_;				/// @brief テクスチャバッファ
	int texHeapNo_;									/// @brief テクスチャディスクリプタヒープ番号

	Size2D size_;			/// @brief テクスチャサイズ

	int displayNum_;		/// @brief 分割テクスチャの何番目を表示するか

	int divideNum_;			/// @brief 分割数
	Size2D divideSize_;		/// @brief 分割後サイズ
	vector<UV> divideUV_;	/// @brief 分割後UV配列
	UV uvOffset_;			/// @brief 分割後UVオフセット
};

