#pragma once

/// @brief �A�v���P�[�V�����E�B���h�E
class Window
{
public:
	
	/// @brief �E�B���h�E�̍쐬
	/// @param width ����
	/// @param height �c��
	/// @param titleName �^�C�g��
	/// @param windowClassName �N���X
	/// @return �����̐���
	virtual bool Create(int width, int height, const std::wstring& titleName, const std::wstring& windowClassName);

	/// @brief �E�B���h�E���b�Z�[�W����
	/// @return �I�����b�Z�[�W��������false������
	bool ProcessMessage();

	/// @brief �������
	void Destroy();

	/// @brief �E�B���h�E�n���h���擾
	/// @return �E�B���h�E�n���h��
	[[nodiscard]] HWND GetHWND() const { return hWnd_; }

	/// @brief �A�v���P�[�V�����n���h���擾
	/// @return �A�v���P�[�V�����n���h��
	[[nodiscard]] HINSTANCE GetHInstance() const { return hInst_; }

#ifdef _DEBUG

	/// @brief ImGui������
	bool InitializeImGui();

	/// @brief ImGui�`�揀��
	void RenderImGuiBigin();

	/// @brief ImGui�`��I��
	void RenderImGuiEnd();

#endif

	/// @brief ���z�t���X�N���[����
	void SetVirtualFullScreen();

	/// @brief �ʏ�X�N���[����
	void SetRestoreWindow();

	/// @brief �f�B�X�v���C�𑜓x�擾
	/// @return �f�B�X�v���C�𑜓x
	[[nodiscard]] const Size2D& GetDisplaySize()const { return displaySize_; }

private:

	HWND hWnd_;					/// @brief �E�B���h�E�n���h��
	HINSTANCE hInst_;			/// @brief �A�v���P�[�V����Instance�n���h��
	RECT windowRect_;			/// @brief �E�B���h�E�T�C�Y
	int isFullScreen_ = false;	/// @brief �t���X�N���[���t���O
	Vector2 mouseMovePow_;		/// @brief �}�E�X�ړ���

	Size2D displaySize_;		/// @brief �f�B�X�v���C�T�C�Y


#ifdef _DEBUG
	ComPtr<ID3D12DescriptorHeap> ImGuiDescHeap_;	/// @brief ImGui�p�f�B�X�N���v�q�[�v
#endif

};

