#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "Texture.h"

struct TexRGBA
{
	unsigned char r, g, b, a;
};

bool IsWICExtension(const string& ext)
{
	return (ext == "jpg"	|| ext == "jpeg"	|| ext == "png" ||
			ext == "bmp"	|| ext == "gif"		|| ext == "tif" ||
			ext == "tiff"	|| ext == "ico"		|| ext == "heif"||
			ext == "webp");
}

Texture::Texture()
{
	CreateDummyTexture();
}

Texture::Texture(const string& filePath)
{
	// 拡張子取得
	string ext = GetExtension(filePath);

	if (IsWICExtension(ext))
	{
		LoadWICTexture(filePath);
	}
	else if (ext == "dds")
	{
		LoadDDSTexture(filePath);
	}
	else if (ext == "tga")
	{
		LoadWICTexture(filePath);
	}
	else
	{
		CreateColorTexture(XMFLOAT4(0.0f,0.0f,0.0f,1.0f));
	}
}

Texture::Texture(XMFLOAT4 color)
{
	CreateColorTexture(color);
}

Texture::Texture(TEXTURE_DATA data)
{
	CreateTextureBuffer(data);
}

Texture::~Texture()
{
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(texHeapNo_);
}

void Texture::Release()
{
}

void Texture::DivideImage(int xNum, int yNum)
{
	divideNum_ = xNum * yNum;
	uvOffset_.u = (size_.x / xNum) / size_.x;
	uvOffset_.v = (size_.y / yNum) / size_.y;
	divideSize_.x = size_.x / xNum;
	divideSize_.y = size_.y / yNum;

	divideUV_.clear();

	for (int y = 0; y < yNum; y++) {
		for (int x = 0; x < xNum; x++) {
			UV uv = { uvOffset_.u * x, uvOffset_.v * y };
			divideUV_.emplace_back(uv);
		}
	}
}

void Texture::SetTexture(int rootIndex) const
{
	DxCommand->SetGraphicsRootDescriptorTable(rootIndex, HeapAllocator.GetCBV_SRV_UAVGPUAddress(texHeapNo_));
}

void Texture::LoadWICTexture(const string& filePath)
{
	//WICテクスチャのロード
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	// WICテクスチャのロード
	HRESULT hr = LoadFromWICFile(cp_to_wide(filePath).c_str(), WIC_FLAGS_NONE, &metadata, scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0);

	if (img == nullptr) {

		// テクスチャが読み込めていない場合
		// ダミーテクスチャを作成
		CreateDummyTexture();

		return;
	}

	size_.x = (float)img->width;
	size_.y = (float)img->height;
	divideSize_.x = (float)img->width;
	divideSize_.y = (float)img->height;
	divideUV_.emplace_back(UV(0.0f, 0.0f));
	uvOffset_.u = 1.0f;
	uvOffset_.v = 1.0f;

	TEXTURE_DATA data;
	data.format = metadata.format;												// @brief RGBAフォーマット
	data.width = static_cast<UINT64>(metadata.width);							/// @brief 幅
	data.height = static_cast<UINT>(metadata.height);							/// @brief 高さ
	data.arraySize = static_cast<UINT16>(metadata.arraySize);					/// @brief 配列サイズ
	data.mipLevels = static_cast<UINT16>(metadata.mipLevels);					/// @brief ミップ数
	data.dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	/// @brief ディメンション
	data.pixels = img->pixels;													/// @brief 書き込みデータの先頭アドレス
	data.rowPitch = static_cast <UINT>(img->rowPitch);							/// @brief 1行当たりのデータサイズ
	data.slicePitch = static_cast <UINT>(img->slicePitch);						/// @brief スライスあたりのデータサイズ
	data.scratchImg = &scratchImg;												/// @brief 読み込みデータ

	CreateTextureBuffer(data);
}

void Texture::LoadDDSTexture(const string& filePath)
{
	//WICテクスチャのロード
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	// WICテクスチャのロード
	HRESULT hr = LoadFromDDSFile(cp_to_wide(filePath).c_str(), DDS_FLAGS_NONE, &metadata, scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0);

	if (img == nullptr) {

		// テクスチャが読み込めていない場合
		// ダミーテクスチャを作成
		CreateDummyTexture();

		return;
	}

	size_.x = (float)img->width;
	size_.y = (float)img->height;
	divideSize_.x = (float)img->width;
	divideSize_.y = (float)img->height;
	divideUV_.emplace_back(UV(0.0f, 0.0f));
	uvOffset_.u = 1.0f;
	uvOffset_.v = 1.0f;

	TEXTURE_DATA data;
	data.format = metadata.format;												// @brief RGBAフォーマット
	data.width = static_cast<UINT64>(metadata.width);							/// @brief 幅
	data.height = static_cast<UINT>(metadata.height);							/// @brief 高さ
	data.arraySize = static_cast<UINT16>(metadata.arraySize);					/// @brief 配列サイズ
	data.mipLevels = static_cast<UINT16>(metadata.mipLevels);					/// @brief ミップ数
	data.dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	/// @brief ディメンション
	data.pixels = img->pixels;													/// @brief 書き込みデータの先頭アドレス
	data.rowPitch = static_cast <UINT>(img->rowPitch);							/// @brief 1行当たりのデータサイズ
	data.slicePitch = static_cast <UINT>(img->slicePitch);						/// @brief スライスあたりのデータサイズ
	data.scratchImg = &scratchImg;												/// @brief 読み込みデータ

	CreateTextureBuffer(data);
}

void Texture::CreateColorTexture(XMFLOAT4 color)
{
	int texWidth = 1, texHeight = 1;

	size_.x = (float)texWidth;
	size_.y = (float)texHeight;
	divideSize_.x = (float)texWidth;
	divideSize_.y = (float)texHeight;
	divideUV_.emplace_back(UV(0.0f, 0.0f));
	uvOffset_.u = 1.0f;
	uvOffset_.v = 1.0f;

	TexRGBA texData;
	texData.r = static_cast<unsigned char>(color.x);
	texData.g = static_cast<unsigned char>(color.y);
	texData.b = static_cast<unsigned char>(color.z);
	texData.a = static_cast<unsigned char>(color.w);

	TEXTURE_DATA data;
	data.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;						/// @brief RGBAフォーマット
	data.width = static_cast<UINT64>(texWidth);									/// @brief 幅
	data.height = static_cast<UINT>(texHeight);									/// @brief 高さ
	data.arraySize = static_cast<UINT16>(1);									/// @brief 配列サイズ
	data.mipLevels = static_cast<UINT16>(1);									/// @brief ミップ数
	data.dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;						/// @brief ディメンション
	data.pixels = &texData;														/// @brief 書き込みデータの先頭アドレス
	data.rowPitch = static_cast <UINT>(sizeof(TexRGBA));						/// @brief 1行当たりのデータサイズ
	data.slicePitch = static_cast <UINT>(sizeof(TexRGBA));						/// @brief スライスあたりのデータサイズ


	CreateTextureBuffer(data);
}

void Texture::CreateDummyTexture()
{
	int texWidth = 256, texHeight = 256;

	size_.x = (float)texWidth;
	size_.y = (float)texHeight;
	divideSize_.x = (float)texWidth;
	divideSize_.y = (float)texHeight;
	divideUV_.emplace_back(UV(0.0f, 0.0f));
	uvOffset_.u = 1.0f;
	uvOffset_.v = 1.0f;

	vector<TexRGBA> texData(texWidth * texHeight);

	// ランダムテクスチャ
	for (auto& pixel : texData)
	{
		pixel.r = GetRand(0, 255);
		pixel.g = GetRand(0, 255);
		pixel.b = GetRand(0, 255);
		pixel.a = 255;
	}

	TEXTURE_DATA data;
	data.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;						/// @brief RGBAフォーマット
	data.width = static_cast<UINT64>(texWidth);									/// @brief 幅
	data.height = static_cast<UINT>(texHeight);									/// @brief 高さ
	data.arraySize = static_cast<UINT16>(1);									/// @brief 配列サイズ
	data.mipLevels = static_cast<UINT16>(1);									/// @brief ミップ数
	data.dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;						/// @brief ディメンション
	data.pixels = texData.data();												/// @brief 書き込みデータの先頭アドレス
	data.rowPitch = static_cast <UINT>(sizeof(TexRGBA) * texWidth);			/// @brief 1行当たりのデータサイズ
	data.slicePitch = static_cast <UINT>(sizeof(TexRGBA) * texData.size());	/// @brief スライスあたりのデータサイズ

	CreateTextureBuffer(data);
}

void Texture::CreateTextureBuffer(TEXTURE_DATA data)
{
	// テクスチャデータ作成
	{
		// テクスチャバッファ作成
		//WriteToSubresourceで転送する用のヒープ設定
		D3D12_HEAP_PROPERTIES texHeapProp = {};
		texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//特殊な設定なのでdefaultでもuploadでもなく
		texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//ライトバックで
		texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//転送がL0つまりCPU側から直で
		texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
		texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
		
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Format = data.format;//RGBAフォーマット
		resDesc.Width = data.width;//幅
		resDesc.Height = data.height;//高さ
		resDesc.DepthOrArraySize = data.arraySize;//2Dで配列でもないので１
		resDesc.SampleDesc.Count = 1;//通常テクスチャなのでアンチェリしない
		resDesc.SampleDesc.Quality = 0;//
		resDesc.MipLevels = data.mipLevels;
		resDesc.Dimension = data.dimension;//2Dテクスチャ用
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//レイアウトについては決定しない
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//とくにフラグなし

		MainDevice->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,//特に指定なし
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//テクスチャ用(ピクセルシェーダから見る用)
			nullptr,
			IID_PPV_ARGS(texBuffer_.ReleaseAndGetAddressOf())
		);

		// 全ミップレベルのデータを転送

		if (data.mipLevels == 1)
		{
			texBuffer_->WriteToSubresource(0,
				nullptr,//全領域へコピー
				data.pixels,//元データアドレス
				data.rowPitch,//1ラインサイズ
				data.slicePitch//全サイズ
			);
		}
		else
		{
			for (UINT mip = 0; mip < data.mipLevels; ++mip) {
				auto img = data.scratchImg->GetImage(static_cast<size_t>(mip), 0, 0);
				texBuffer_->WriteToSubresource(
					mip,
					nullptr,
					img->pixels,
					static_cast<UINT>(img->rowPitch),
					static_cast<UINT>(img->slicePitch)
				);
			}
		}

		//通常テクスチャビュー作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = data.format;//RGBA(0.0f～1.0fに正規化)
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = data.mipLevels;

		texHeapNo_ = HeapAllocator.CreateSRV(texBuffer_.Get(), srvDesc);
	}
}
