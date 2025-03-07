#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define Factory DXFactory::GetInstance().GetFactory()

/// @brief �t�@�N�g���[�Ǘ�
class DXFactory final : public StaticSingleton<DXFactory>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief Factory�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief Factory�擾
	/// @return Factory
	[[nodiscard]] const ComPtr<IDXGIFactory6>& GetFactory();

private:

	DXFactory();
	~DXFactory();
	PROOF_OF_STATIC_SINGLETON(DXFactory);

	ComPtr<IDXGIFactory6> factory_;		/// @brief �t�@�N�g���[
};

