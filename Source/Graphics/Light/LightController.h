#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "HemisphereLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#define Light LightController::GetInstance()

/// @brief ライト管理
class LightController final : public StaticSingleton<LightController>
{
public:

	static constexpr int LIGHT_CAMERA_MAX = 10;

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 更新処理
	void Update();

	/// @brief ライト情報をシェーダにセット
	void SetBeforeDraw();


	/// @brief 半球ライトを追加
	/// @param light ライト情報
	/// @param key キー
	/// @param isCalcMat ライトカメラ行列を作成するか否か
	void AddHemisphereLight(HEMISPHERE_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief 指向性ライトを追加
	/// @param light ライト情報
	/// @param key キー
	/// @param isCalcMat ライトカメラ行列を作成するか否か
	void AddDirectionalLight(DIRECTIONAL_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief ポイントライトを追加
	/// @param light ライト情報
	/// @param key キー
	/// @param isCalcMat ライトカメラ行列を作成するか否か
	void AddPointLight(POINT_LIGHT light, const string& key, bool isCalcMat = false);

	/// @brief スポットライトを追加
	/// @param light ライト情報
	/// @param key キー
	/// @param isCalcMat ライトカメラ行列を作成するか否か
	void AddSpotLight(SPOT_LIGHT light, const string& key, bool isCalcMat = false);


	/// @brief 半球ライト情報を変更
	/// @param light ライト情報
	/// @param key キー
	void SetHemisphereLightInfo(HEMISPHERE_LIGHT light, const string& key);

	/// @brief 指向性ライト情報を変更
	/// @param light ライト情報
	/// @param key キー
	void SetDirectionalLightInfo(DIRECTIONAL_LIGHT light, const string& key);

	/// @brief ポイントライト情報を変更
	/// @param light ライト情報
	/// @param key キー
	void SetPointLightInfo(POINT_LIGHT light, const string& key);

	/// @brief スポットライト情報を変更
	/// @param light ライト情報
	/// @param key キー
	void SetSpotLightInfo(SPOT_LIGHT light, const string& key);


	/// @brief 指定の半球ライトを削除
	/// @param key キー
	void DeleteHemisphereLightInfo(const string& key);

	/// @brief 指定の指向性ライトを削除
	/// @param key キー
	void DeleteDirectionalLightInfo(const string& key);

	/// @brief 指定のポイントライトを削除
	/// @param key キー
	void DeletePointLightInfo(const string& key);

	/// @brief 指定のスポットライトを削除
	/// @param key キー
	void DeleteSpotLightInfo(const string& key);

	/// @brief 使用しているライトカメラ行列の数を返す
	/// @return 使用しているライトカメラ行列の数
	[[nodiscard]] const UINT& GetLightCameraProjMatNum()const { return useLightCameraNum_; }

	/// @brief 指定のライトカメラ行列を取得
	/// @param no 番号
	/// @return ライトカメラ行列
	[[nodiscard]] const XMMATRIX& GetLightCameraProjMat(int no)const;

private:

	struct LIGHT_NUM;

	LightController();
	~LightController();
	PROOF_OF_STATIC_SINGLETON(LightController);

	/// @brief ライト数用定数バッファ作成
	void CreateConstantBufferLightNum();

	/// @brief 半球ライト用定数バッファ作成
	void CreateConstantBufferHemisphereLight();

	/// @brief 指向性ライト用定数バッファ作成
	void CreateConstantBufferDirectionalLight();

	/// @brief ポイントライト用定数バッファ作成
	void CreateConstantBufferPointLight();

	/// @brief スポットライト用定数バッファ作成
	void CreateConstantBufferSpotLight();

	/// @brief 半球ライトカメラ行列を計算
	void CalculateHemisphereLightViewProjectionMatrix();

	/// @brief 指向性ライトカメラ行列を計算
	void CalculateDirectionalLightViewProjectionMatrix();

	/// @brief ポイントライトカメラ行列を計算
	void CalculatePointLightViewProjectionMatrix();

	/// @brief スポットライトカメラ行列を計算
	void CalculateSpotLightViewProjectionMatrix();

	unordered_map<string, unique_ptr<HEMISPHERE_LIGHT>> hemisphereLights_;			/// @brief 半球ライト
	unordered_map<string, unique_ptr<DIRECTIONAL_LIGHT>> directionalLights_;		/// @brief 指向性ライト
	unordered_map<string, unique_ptr<POINT_LIGHT>> pointLights_;					/// @brief ポイントライト
	unordered_map<string, unique_ptr<SPOT_LIGHT>> spotLights_;						/// @brief スポットライト

	UINT useLightCameraNum_;									/// @brief 現在使用しているライトカメラ行列の数

	ComPtr<ID3D12Resource> lightNumConstBuffer_;				/// @brief ライト数情報バッファ
	int lightNumHeapNo_;										/// @brief ライト数情報ディスクリプタヒープ番号
	LIGHT_NUM* lightNumMap_;									/// @brief バッファ更新用変数

	ComPtr<ID3D12Resource> hemisphereLightShaderResource_;		/// @brief 半球ライト情報バッファ
	int hemisphereLightHeapNo_;									/// @brief 半球ライト情報ディスクリプタヒープ番号
	HEMISPHERE_LIGHT* hemisphereLightMap_;						/// @brief バッファ更新用変数

	ComPtr<ID3D12Resource> directionalLightShaderResource_;		/// @brief 指向性ライト情報バッファ
	int directionalLightHeapNo_;								/// @brief 指向性ライト情報ディスクリプタヒープ番号
	DIRECTIONAL_LIGHT* directionalLightMap_;					/// @brief バッファ更新用変数

	ComPtr<ID3D12Resource> pointLightShaderResource_;			/// @brief ポイントライト情報バッファ
	int pointLightHeapNo_;										/// @brief ポイントライト情報ディスクリプタヒープ番号
	POINT_LIGHT* pointLightMap_;								/// @brief バッファ更新用変数

	ComPtr<ID3D12Resource> spotLightShaderResource_;			/// @brief スポットライト情報バッファ
	int spotLightHeapNo_;										/// @brief スポットライト情報ディスクリプタヒープ番号
	SPOT_LIGHT* spotLightMap_;									/// @brief バッファ更新用変数

	vector<string> hemisphereLightCameraProjMatCalcLights_;		/// @brief 半球ライトカメラ行列を計算するライト
	vector<string> directionalLightCameraProjMatCalcLights_;	/// @brief 指向性ライトカメラ行列を計算するライト
	vector<string> pointLightCameraProjMatCalcLights_;			/// @brief ポイントライトカメラ行列を計算するライト
	vector<string> spotLightCameraProjMatCalcLights_;			/// @brief スポットライトカメラ行列を計算するライト
};

struct LightController::LIGHT_NUM
{
	UINT hemisphereLightNum = 0;		/// @brief 半球ライト数
	UINT directionalLightNum = 0;		/// @brief 指向性ライト数
	UINT pointLightNum = 0;				/// @brief ポイントライト数
	UINT spotLightNum = 0;				/// @brief スポットライト数

	UINT useLightCameraNum = 0;								/// @brief 使用しているライトカメラ数
	XMMATRIX lightCameraMatrix[LIGHT_CAMERA_MAX] = {};		/// @brief ライトカメラ行列
	XMMATRIX lightCameraViewMatrix[LIGHT_CAMERA_MAX] = {};	/// @brief ライトカメラビュー行列
	XMMATRIX lightCameraProjMatrix[LIGHT_CAMERA_MAX] = {};	/// @brief ライトカメラプロジェクション行列
};
