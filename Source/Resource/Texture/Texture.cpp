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
	// �g���q�擾
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
	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	// WIC�e�N�X�`���̃��[�h
	HRESULT hr = LoadFromWICFile(cp_to_wide(filePath).c_str(), WIC_FLAGS_NONE, &metadata, scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0);

	if (img == nullptr) {

		// �e�N�X�`�����ǂݍ��߂Ă��Ȃ��ꍇ
		// �_�~�[�e�N�X�`�����쐬
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
	data.format = metadata.format;												// @brief RGBA�t�H�[�}�b�g
	data.width = static_cast<UINT64>(metadata.width);							/// @brief ��
	data.height = static_cast<UINT>(metadata.height);							/// @brief ����
	data.arraySize = static_cast<UINT16>(metadata.arraySize);					/// @brief �z��T�C�Y
	data.mipLevels = static_cast<UINT16>(metadata.mipLevels);					/// @brief �~�b�v��
	data.dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	/// @brief �f�B�����V����
	data.pixels = img->pixels;													/// @brief �������݃f�[�^�̐擪�A�h���X
	data.rowPitch = static_cast <UINT>(img->rowPitch);							/// @brief 1�s������̃f�[�^�T�C�Y
	data.slicePitch = static_cast <UINT>(img->slicePitch);						/// @brief �X���C�X������̃f�[�^�T�C�Y
	data.scratchImg = &scratchImg;												/// @brief �ǂݍ��݃f�[�^

	CreateTextureBuffer(data);
}

void Texture::LoadDDSTexture(const string& filePath)
{
	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	// WIC�e�N�X�`���̃��[�h
	HRESULT hr = LoadFromDDSFile(cp_to_wide(filePath).c_str(), DDS_FLAGS_NONE, &metadata, scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0);

	if (img == nullptr) {

		// �e�N�X�`�����ǂݍ��߂Ă��Ȃ��ꍇ
		// �_�~�[�e�N�X�`�����쐬
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
	data.format = metadata.format;												// @brief RGBA�t�H�[�}�b�g
	data.width = static_cast<UINT64>(metadata.width);							/// @brief ��
	data.height = static_cast<UINT>(metadata.height);							/// @brief ����
	data.arraySize = static_cast<UINT16>(metadata.arraySize);					/// @brief �z��T�C�Y
	data.mipLevels = static_cast<UINT16>(metadata.mipLevels);					/// @brief �~�b�v��
	data.dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	/// @brief �f�B�����V����
	data.pixels = img->pixels;													/// @brief �������݃f�[�^�̐擪�A�h���X
	data.rowPitch = static_cast <UINT>(img->rowPitch);							/// @brief 1�s������̃f�[�^�T�C�Y
	data.slicePitch = static_cast <UINT>(img->slicePitch);						/// @brief �X���C�X������̃f�[�^�T�C�Y
	data.scratchImg = &scratchImg;												/// @brief �ǂݍ��݃f�[�^

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
	data.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;						/// @brief RGBA�t�H�[�}�b�g
	data.width = static_cast<UINT64>(texWidth);									/// @brief ��
	data.height = static_cast<UINT>(texHeight);									/// @brief ����
	data.arraySize = static_cast<UINT16>(1);									/// @brief �z��T�C�Y
	data.mipLevels = static_cast<UINT16>(1);									/// @brief �~�b�v��
	data.dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;						/// @brief �f�B�����V����
	data.pixels = &texData;														/// @brief �������݃f�[�^�̐擪�A�h���X
	data.rowPitch = static_cast <UINT>(sizeof(TexRGBA));						/// @brief 1�s������̃f�[�^�T�C�Y
	data.slicePitch = static_cast <UINT>(sizeof(TexRGBA));						/// @brief �X���C�X������̃f�[�^�T�C�Y


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

	// �����_���e�N�X�`��
	for (auto& pixel : texData)
	{
		pixel.r = GetRand(0, 255);
		pixel.g = GetRand(0, 255);
		pixel.b = GetRand(0, 255);
		pixel.a = 255;
	}

	TEXTURE_DATA data;
	data.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;						/// @brief RGBA�t�H�[�}�b�g
	data.width = static_cast<UINT64>(texWidth);									/// @brief ��
	data.height = static_cast<UINT>(texHeight);									/// @brief ����
	data.arraySize = static_cast<UINT16>(1);									/// @brief �z��T�C�Y
	data.mipLevels = static_cast<UINT16>(1);									/// @brief �~�b�v��
	data.dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;						/// @brief �f�B�����V����
	data.pixels = texData.data();												/// @brief �������݃f�[�^�̐擪�A�h���X
	data.rowPitch = static_cast <UINT>(sizeof(TexRGBA) * texWidth);			/// @brief 1�s������̃f�[�^�T�C�Y
	data.slicePitch = static_cast <UINT>(sizeof(TexRGBA) * texData.size());	/// @brief �X���C�X������̃f�[�^�T�C�Y

	CreateTextureBuffer(data);
}

void Texture::CreateTextureBuffer(TEXTURE_DATA data)
{
	// �e�N�X�`���f�[�^�쐬
	{
		// �e�N�X�`���o�b�t�@�쐬
		//WriteToSubresource�œ]������p�̃q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES texHeapProp = {};
		texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//����Ȑݒ�Ȃ̂�default�ł�upload�ł��Ȃ�
		texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//���C�g�o�b�N��
		texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�]����L0�܂�CPU�����璼��
		texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
		texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
		
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Format = data.format;//RGBA�t�H�[�}�b�g
		resDesc.Width = data.width;//��
		resDesc.Height = data.height;//����
		resDesc.DepthOrArraySize = data.arraySize;//2D�Ŕz��ł��Ȃ��̂łP
		resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
		resDesc.SampleDesc.Quality = 0;//
		resDesc.MipLevels = data.mipLevels;
		resDesc.Dimension = data.dimension;//2D�e�N�X�`���p
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�

		MainDevice->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//�e�N�X�`���p(�s�N�Z���V�F�[�_���猩��p)
			nullptr,
			IID_PPV_ARGS(texBuffer_.ReleaseAndGetAddressOf())
		);

		// �S�~�b�v���x���̃f�[�^��]��

		if (data.mipLevels == 1)
		{
			texBuffer_->WriteToSubresource(0,
				nullptr,//�S�̈�փR�s�[
				data.pixels,//���f�[�^�A�h���X
				data.rowPitch,//1���C���T�C�Y
				data.slicePitch//�S�T�C�Y
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

		//�ʏ�e�N�X�`���r���[�쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = data.format;//RGBA(0.0f�`1.0f�ɐ��K��)
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = data.mipLevels;

		texHeapNo_ = HeapAllocator.CreateSRV(texBuffer_.Get(), srvDesc);
	}
}
