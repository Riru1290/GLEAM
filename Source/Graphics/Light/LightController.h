#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "HemisphereLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#define Light LightController::GetInstance()

/// @brief ���C�g�Ǘ�
class LightController final : public StaticSingleton<LightController>
{
public:

	static constexpr int LIGHT_CAMERA_MAX = 10;

	/// @brief �������
	void Terminate() override;

	/// @brief �X�V����
	void Update();

	/// @brief ���C�g�����V�F�[�_�ɃZ�b�g
	void SetBeforeDraw();


	/// @brief �������C�g��ǉ�
	/// @param light ���C�g���
	/// @param key �L�[
	/// @param isCalcMat ���C�g�J�����s����쐬���邩�ۂ�
	void AddHemisphereLight(HEMISPHERE_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief �w�������C�g��ǉ�
	/// @param light ���C�g���
	/// @param key �L�[
	/// @param isCalcMat ���C�g�J�����s����쐬���邩�ۂ�
	void AddDirectionalLight(DIRECTIONAL_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief �|�C���g���C�g��ǉ�
	/// @param light ���C�g���
	/// @param key �L�[
	/// @param isCalcMat ���C�g�J�����s����쐬���邩�ۂ�
	void AddPointLight(POINT_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief �X�|�b�g���C�g��ǉ�
	/// @param light ���C�g���
	/// @param key �L�[
	/// @param isCalcMat ���C�g�J�����s����쐬���邩�ۂ�
	void AddSpotLight(SPOT_LIGHT light, const string& key, bool isCalcMat = false);


	/// @brief �������C�g����ύX
	/// @param light ���C�g���
	/// @param key �L�[
	void SetHemisphereLightInfo(HEMISPHERE_LIGHT light, const string& key);

	/// @brief �w�������C�g����ύX
	/// @param light ���C�g���
	/// @param key �L�[
	void SetDirectionalLightInfo(DIRECTIONAL_LIGHT light, const string& key);

	/// @brief �|�C���g���C�g����ύX
	/// @param light ���C�g���
	/// @param key �L�[
	void SetPointLightInfo(POINT_LIGHT light, const string& key);

	/// @brief �X�|�b�g���C�g����ύX
	/// @param light ���C�g���
	/// @param key �L�[
	void SetSpotLightInfo(SPOT_LIGHT light, const string& key);


	/// @brief �w��̔������C�g���폜
	/// @param key �L�[
	void DeleteHemisphereLightInfo(const string& key);

	/// @brief �w��̎w�������C�g���폜
	/// @param key �L�[
	void DeleteDirectionalLightInfo(const string& key);

	/// @brief �w��̃|�C���g���C�g���폜
	/// @param key �L�[
	void DeletePointLightInfo(const string& key);

	/// @brief �w��̃X�|�b�g���C�g���폜
	/// @param key �L�[
	void DeleteSpotLightInfo(const string& key);

	/// @brief �g�p���Ă��郉�C�g�J�����s��̐���Ԃ�
	/// @return �g�p���Ă��郉�C�g�J�����s��̐�
	[[nodiscard]] const UINT& GetLightCameraProjMatNum()const { return useLightCameraNum_; }

	/// @brief �w��̃��C�g�J�����s����擾
	/// @param no �ԍ�
	/// @return ���C�g�J�����s��
	[[nodiscard]] const XMMATRIX& GetLightCameraProjMat(int no)const;

private:

	struct LIGHT_NUM;

	LightController();
	~LightController();
	PROOF_OF_STATIC_SINGLETON(LightController);

	/// @brief ���C�g���p�萔�o�b�t�@�쐬
	void CreateConstantBufferLightNum();

	/// @brief �������C�g�p�萔�o�b�t�@�쐬
	void CreateConstantBufferHemisphereLight();

	/// @brief �w�������C�g�p�萔�o�b�t�@�쐬
	void CreateConstantBufferDirectionalLight();

	/// @brief �|�C���g���C�g�p�萔�o�b�t�@�쐬
	void CreateConstantBufferPointLight();

	/// @brief �X�|�b�g���C�g�p�萔�o�b�t�@�쐬
	void CreateConstantBufferSpotLight();

	/// @brief �������C�g�J�����s����v�Z
	void CalculateHemisphereLightViewProjectionMatrix();

	/// @brief �w�������C�g�J�����s����v�Z
	void CalculateDirectionalLightViewProjectionMatrix();

	/// @brief �|�C���g���C�g�J�����s����v�Z
	void CalculatePointLightViewProjectionMatrix();

	/// @brief �X�|�b�g���C�g�J�����s����v�Z
	void CalculateSpotLightViewProjectionMatrix();

	unordered_map<string, unique_ptr<HEMISPHERE_LIGHT>> hemisphereLights_;			/// @brief �������C�g
	unordered_map<string, unique_ptr<DIRECTIONAL_LIGHT>> directionalLights_;		/// @brief �w�������C�g
	unordered_map<string, unique_ptr<POINT_LIGHT>> pointLights_;					/// @brief �|�C���g���C�g
	unordered_map<string, unique_ptr<SPOT_LIGHT>> spotLights_;						/// @brief �X�|�b�g���C�g

	UINT useLightCameraNum_;									/// @brief ���ݎg�p���Ă��郉�C�g�J�����s��̐�

	ComPtr<ID3D12Resource> lightNumConstBuffer_;				/// @brief ���C�g�����o�b�t�@
	int lightNumHeapNo_;										/// @brief ���C�g�����f�B�X�N���v�^�q�[�v�ԍ�
	LIGHT_NUM* lightNumMap_;									/// @brief �o�b�t�@�X�V�p�ϐ�

	ComPtr<ID3D12Resource> hemisphereLightShaderResource_;		/// @brief �������C�g���o�b�t�@
	int hemisphereLightHeapNo_;									/// @brief �������C�g���f�B�X�N���v�^�q�[�v�ԍ�
	HEMISPHERE_LIGHT* hemisphereLightMap_;						/// @brief �o�b�t�@�X�V�p�ϐ�

	ComPtr<ID3D12Resource> directionalLightShaderResource_;		/// @brief �w�������C�g���o�b�t�@
	int directionalLightHeapNo_;								/// @brief �w�������C�g���f�B�X�N���v�^�q�[�v�ԍ�
	DIRECTIONAL_LIGHT* directionalLightMap_;					/// @brief �o�b�t�@�X�V�p�ϐ�

	ComPtr<ID3D12Resource> pointLightShaderResource_;			/// @brief �|�C���g���C�g���o�b�t�@
	int pointLightHeapNo_;										/// @brief �|�C���g���C�g���f�B�X�N���v�^�q�[�v�ԍ�
	POINT_LIGHT* pointLightMap_;								/// @brief �o�b�t�@�X�V�p�ϐ�

	ComPtr<ID3D12Resource> spotLightShaderResource_;			/// @brief �X�|�b�g���C�g���o�b�t�@
	int spotLightHeapNo_;										/// @brief �X�|�b�g���C�g���f�B�X�N���v�^�q�[�v�ԍ�
	SPOT_LIGHT* spotLightMap_;									/// @brief �o�b�t�@�X�V�p�ϐ�

	vector<string> hemisphereLightCameraProjMatCalcLights_;		/// @brief �������C�g�J�����s����v�Z���郉�C�g
	vector<string> directionalLightCameraProjMatCalcLights_;	/// @brief �w�������C�g�J�����s����v�Z���郉�C�g
	vector<string> pointLightCameraProjMatCalcLights_;			/// @brief �|�C���g���C�g�J�����s����v�Z���郉�C�g
	vector<string> spotLightCameraProjMatCalcLights_;			/// @brief �X�|�b�g���C�g�J�����s����v�Z���郉�C�g
};

struct LightController::LIGHT_NUM
{
	UINT hemisphereLightNum = 0;		/// @brief �������C�g��
	UINT directionalLightNum = 0;		/// @brief �w�������C�g��
	UINT pointLightNum = 0;				/// @brief �|�C���g���C�g��
	UINT spotLightNum = 0;				/// @brief �X�|�b�g���C�g��

	UINT useLightCameraNum = 0;								/// @brief �g�p���Ă��郉�C�g�J������
	XMMATRIX lightCameraMatrix[LIGHT_CAMERA_MAX] = {};		/// @brief ���C�g�J�����s��
	XMMATRIX lightCameraViewMatrix[LIGHT_CAMERA_MAX] = {};	/// @brief ���C�g�J�����r���[�s��
	XMMATRIX lightCameraProjMatrix[LIGHT_CAMERA_MAX] = {};	/// @brief ���C�g�J�����v���W�F�N�V�����s��
};
