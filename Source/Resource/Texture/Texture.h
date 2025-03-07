#pragma once
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib") 
#include "../Resource.h"

/// @brief �e�N�X�`���f�[�^
struct TEXTURE_DATA
{
	DXGI_FORMAT format;						/// @brief RGBA�t�H�[�}�b�g
	UINT64 width;							/// @brief ��
	UINT height;							/// @brief ����
	UINT16 arraySize;						/// @brief �z��T�C�Y
	UINT16 mipLevels;						/// @brief �~�b�v��
	D3D12_RESOURCE_DIMENSION dimension;		/// @brief �f�B�����V����
	void* pixels;							/// @brief �������݃f�[�^�̐擪�A�h���X
	UINT rowPitch;							/// @brief 1�s������̃f�[�^�T�C�Y
	UINT slicePitch;						/// @brief �X���C�X������̃f�[�^�T�C�Y
	ScratchImage* scratchImg;				/// @brief �ǂݍ��݃f�[�^
};

/// @brief �e�N�X�`��
class Texture final : public Resource
{
public:

	[[nodiscard]]Texture();
	[[nodiscard]]Texture(const string& filePath);
	[[nodiscard]]Texture(XMFLOAT4 color);
	[[nodiscard]]Texture(TEXTURE_DATA data);
	~Texture();

	/// @brief �������
	void Release()override;

	/// @brief �e�N�X�`������
	/// @param xNum X������
	/// @param yNum Y������
	void DivideImage(int xNum, int yNum);

	/// @brief �������擾
	/// @return ������
	[[nodiscard]] const int& GetDivideNum()const { return divideNum_; }

	/// @brief �e�N�X�`���T�C�Y�擾
	/// @return �e�N�X�`���T�C�Y
	[[nodiscard]] const Size2D& GetTextureSize()const { return size_; }

	/// @brief ������e�N�X�`���T�C�Y�擾
	/// @return ������e�N�X�`���T�C�Y
	[[nodiscard]] const Size2D& GetDivideSize()const { return divideSize_; }

	/// @brief ������UV�I�t�Z�b�g�擾
	/// @return ������UV�I�t�Z�b�g
	[[nodiscard]] const UV& GetUVOffset()const { return uvOffset_; }

	/// @brief ������UV�z��擾
	/// @return ������UV�z��
	[[nodiscard]] const vector<UV>& GetDivideUV()const { return divideUV_; }


	/// @brief �e�N�X�`���f�[�^���Z�b�g
	/// @param rootIndex ���[�g�V�O�l�`���C���f�b�N�X
	void SetTexture(int rootIndex) const;

private:

	/// @brief WIC�Ή��摜�ǂݍ���
	void LoadWICTexture(const string& filePath);

	/// @brief DDS�摜(�~�b�v�}�b�v)�ǂݍ���
	void LoadDDSTexture(const string& filePath);

	/// @brief �J���[�e�N�X�`���쐬
	void CreateColorTexture(XMFLOAT4 color);

	/// @brief �_�~�[�e�N�X�`���쐬
	void CreateDummyTexture();

	/// @brief �e�N�X�`���o�b�t�@�쐬
	/// @param data �e�N�X�`���f�[�^
	void CreateTextureBuffer(TEXTURE_DATA data);


	ComPtr<ID3D12Resource> texBuffer_;				/// @brief �e�N�X�`���o�b�t�@
	int texHeapNo_;									/// @brief �e�N�X�`���f�B�X�N���v�^�q�[�v�ԍ�

	Size2D size_;			/// @brief �e�N�X�`���T�C�Y

	int displayNum_;		/// @brief �����e�N�X�`���̉��Ԗڂ�\�����邩

	int divideNum_;			/// @brief ������
	Size2D divideSize_;		/// @brief ������T�C�Y
	vector<UV> divideUV_;	/// @brief ������UV�z��
	UV uvOffset_;			/// @brief ������UV�I�t�Z�b�g
};

