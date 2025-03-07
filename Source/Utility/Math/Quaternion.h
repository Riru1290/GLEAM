#pragma once

class Quaternion
{
public:

	double x,y,z,w;

	auto operator <=>(const Quaternion&)const = default;

	Quaternion();
	Quaternion(const Vector3& rad);
	Quaternion(double w, double x, double y, double z);
	~Quaternion();

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(const Vector3& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, Vector3 axis);

	// 座標を回転させる
	static Vector3 PosAxis(const Quaternion& q, Vector3 pos);
	Vector3 PosAxis(Vector3 pos) const;

	// クォータニオンからオイラー角へ変換
	static Vector3 ToEuler(const Quaternion& q);
	Vector3 ToEuler() const;

	// クォータニオンから行列へ変換
	static XMMATRIX ToMatrix(const Quaternion& q);
	XMMATRIX ToMatrix() const;

	// 基本ベクトルを取得
	Vector3 GetForward() const;
	Vector3 GetBack() const;
	Vector3 GetRight() const;
	Vector3 GetLeft() const;
	Vector3 GetUp() const;
	Vector3 GetDown() const;

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(Vector3 dir);
	static Quaternion LookRotation(Vector3 dir, Vector3 up);

	// 正規化
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized() const;
	void Normalize();

	// 逆クォータニオン
	Quaternion Inverse() const;

	double Length() const;
	double LengthSquared() const;

	// 球面補間
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);

	static Quaternion Identity(void);

	Vector3 xyz() const;

	static Quaternion ExtractQuaternion(const XMMATRIX& matrix)
	{
		// XMMATRIXからクォータニオンを取得
		XMVECTOR quatVec = XMQuaternionRotationMatrix(matrix);

		// XMVECTOR を (w, x, y, z) に変換
		XMFLOAT4 quatFloat4;
		XMStoreFloat4(&quatFloat4, quatVec);

		// Quaternion(float w, x, y, z) の形式に変換して返す
		return Quaternion(quatFloat4.w, quatFloat4.x, quatFloat4.y, quatFloat4.z);
	}

	// クォータニオンからZ軸回転を0にする関数
	static Quaternion RemoveZRotation(const Quaternion& quat)
	{
		// QuaternionをXMVECTORに変換
		XMVECTOR quatVec = XMVectorSet(
			static_cast<float>(quat.x), 
			static_cast<float>(quat.y), 
			static_cast<float>(quat.z), 
			static_cast<float>(quat.w));

		// クォータニオンからオイラー角（ピッチ、ヨー、ロール）を取得
		XMFLOAT3 eulerAngles;
		XMStoreFloat3(&eulerAngles, XMQuaternionRotationRollPitchYawFromVector(quatVec));

		// Z軸回転（ロール）を0度に設定
		eulerAngles.z = 0.0f;

		// X軸とY軸の回転のみで新しいクォータニオンを作成
		XMVECTOR newQuatVec = XMQuaternionRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);

		// 新しいクォータニオンをQuaternion構造体に変換して返す
		XMFLOAT4 newQuatFloat4;
		XMStoreFloat4(&newQuatFloat4, newQuatVec);
		return Quaternion(
			static_cast<float>(newQuatFloat4.x),
			static_cast<float>(newQuatFloat4.y),
			static_cast<float>(newQuatFloat4.z),
			static_cast<float>(newQuatFloat4.w));
	}

	static Quaternion ConvertToLeftHanded(const Quaternion& rightHandQuat)
	{
		// z成分の符号を反転して左手系クォータニオンに変換
		return Quaternion(rightHandQuat.w, rightHandQuat.x, rightHandQuat.y, -rightHandQuat.z);
	}

	// クォータニオンからXMMATRIXに変換する関数
	static XMMATRIX QuaternionToMatrix(const Quaternion& quat)
	{
		// QuaternionをXMVECTORに変換
		XMVECTOR quatVec = XMVectorSet(
			static_cast<float>(quat.x),
			static_cast<float>(quat.y),
			static_cast<float>(quat.z),
			static_cast<float>(quat.w));

		// クォータニオンを回転行列に変換
		XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(quatVec);

		return rotationMatrix;
	}

	// 現在値を目標値に向けて一定角度ずつ回転する関数
	static Quaternion RotateTowards(const Quaternion& current, const Quaternion& target, double maxAngle) {
		// クォータニオン間の角度を取得
		double angle = AngleBetween(current, target);

		// 目標角度が0または設定角度以内なら直接目標値を返す
		if (angle <= maxAngle) {
			return target;
		}

		// 最大角度分だけ進む
		double t = maxAngle / angle;
		return Slerp(current, target, t);
	}

	// クォータニオン間の角度を計算
	static double AngleBetween(const Quaternion& q1, const Quaternion& q2) {
		Quaternion normQ1 = Normalize(q1);
		Quaternion normQ2 = Normalize(q2);

		// 内積計算
		double dot = normQ1.w * normQ2.w + normQ1.x * normQ2.x + normQ1.y * normQ2.y + normQ1.z * normQ2.z;

		// 内積が負の場合、符号を反転（短い回転経路を取る）
		if (dot < 0.0) {
			dot = -dot;
		}

		// 内積を [-1, 1] の範囲にクランプ（誤差対策）
		dot = std::clamp(dot, -1.0, 1.0);

		// 角度計算
		return 2.0 * std::acos(dot);
	}

private:


	// 基本ベクトルを取得
	Vector3 GetDir(Vector3 dir) const;

};

