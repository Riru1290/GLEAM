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

	// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	static Quaternion Euler(const Vector3& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// �N�H�[�^�j�I���̍���
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// �w�莲���w��p���A��]������
	static Quaternion AngleAxis(double rad, Vector3 axis);

	// ���W����]������
	static Vector3 PosAxis(const Quaternion& q, Vector3 pos);
	Vector3 PosAxis(Vector3 pos) const;

	// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	static Vector3 ToEuler(const Quaternion& q);
	Vector3 ToEuler() const;

	// �N�H�[�^�j�I������s��֕ϊ�
	static XMMATRIX ToMatrix(const Quaternion& q);
	XMMATRIX ToMatrix() const;

	// ��{�x�N�g�����擾
	Vector3 GetForward() const;
	Vector3 GetBack() const;
	Vector3 GetRight() const;
	Vector3 GetLeft() const;
	Vector3 GetUp() const;
	Vector3 GetDown() const;

	// �x�N�g������N�H�[�^�j�I���ɕϊ�
	static Quaternion LookRotation(Vector3 dir);
	static Quaternion LookRotation(Vector3 dir, Vector3 up);

	// ���K��
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized() const;
	void Normalize();

	// �t�N�H�[�^�j�I��
	Quaternion Inverse() const;

	double Length() const;
	double LengthSquared() const;

	// ���ʕ��
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);

	static Quaternion Identity(void);

	Vector3 xyz() const;

	static Quaternion ExtractQuaternion(const XMMATRIX& matrix)
	{
		// XMMATRIX����N�H�[�^�j�I�����擾
		XMVECTOR quatVec = XMQuaternionRotationMatrix(matrix);

		// XMVECTOR �� (w, x, y, z) �ɕϊ�
		XMFLOAT4 quatFloat4;
		XMStoreFloat4(&quatFloat4, quatVec);

		// Quaternion(float w, x, y, z) �̌`���ɕϊ����ĕԂ�
		return Quaternion(quatFloat4.w, quatFloat4.x, quatFloat4.y, quatFloat4.z);
	}

	// �N�H�[�^�j�I������Z����]��0�ɂ���֐�
	static Quaternion RemoveZRotation(const Quaternion& quat)
	{
		// Quaternion��XMVECTOR�ɕϊ�
		XMVECTOR quatVec = XMVectorSet(
			static_cast<float>(quat.x), 
			static_cast<float>(quat.y), 
			static_cast<float>(quat.z), 
			static_cast<float>(quat.w));

		// �N�H�[�^�j�I������I�C���[�p�i�s�b�`�A���[�A���[���j���擾
		XMFLOAT3 eulerAngles;
		XMStoreFloat3(&eulerAngles, XMQuaternionRotationRollPitchYawFromVector(quatVec));

		// Z����]�i���[���j��0�x�ɐݒ�
		eulerAngles.z = 0.0f;

		// X����Y���̉�]�݂̂ŐV�����N�H�[�^�j�I�����쐬
		XMVECTOR newQuatVec = XMQuaternionRotationRollPitchYaw(eulerAngles.x, eulerAngles.y, eulerAngles.z);

		// �V�����N�H�[�^�j�I����Quaternion�\���̂ɕϊ����ĕԂ�
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
		// z�����̕����𔽓]���č���n�N�H�[�^�j�I���ɕϊ�
		return Quaternion(rightHandQuat.w, rightHandQuat.x, rightHandQuat.y, -rightHandQuat.z);
	}

	// �N�H�[�^�j�I������XMMATRIX�ɕϊ�����֐�
	static XMMATRIX QuaternionToMatrix(const Quaternion& quat)
	{
		// Quaternion��XMVECTOR�ɕϊ�
		XMVECTOR quatVec = XMVectorSet(
			static_cast<float>(quat.x),
			static_cast<float>(quat.y),
			static_cast<float>(quat.z),
			static_cast<float>(quat.w));

		// �N�H�[�^�j�I������]�s��ɕϊ�
		XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(quatVec);

		return rotationMatrix;
	}

	// ���ݒl��ڕW�l�Ɍ����Ĉ��p�x����]����֐�
	static Quaternion RotateTowards(const Quaternion& current, const Quaternion& target, double maxAngle) {
		// �N�H�[�^�j�I���Ԃ̊p�x���擾
		double angle = AngleBetween(current, target);

		// �ڕW�p�x��0�܂��͐ݒ�p�x�ȓ��Ȃ璼�ږڕW�l��Ԃ�
		if (angle <= maxAngle) {
			return target;
		}

		// �ő�p�x�������i��
		double t = maxAngle / angle;
		return Slerp(current, target, t);
	}

	// �N�H�[�^�j�I���Ԃ̊p�x���v�Z
	static double AngleBetween(const Quaternion& q1, const Quaternion& q2) {
		Quaternion normQ1 = Normalize(q1);
		Quaternion normQ2 = Normalize(q2);

		// ���όv�Z
		double dot = normQ1.w * normQ2.w + normQ1.x * normQ2.x + normQ1.y * normQ2.y + normQ1.z * normQ2.z;

		// ���ς����̏ꍇ�A�����𔽓]�i�Z����]�o�H�����j
		if (dot < 0.0) {
			dot = -dot;
		}

		// ���ς� [-1, 1] �͈̔͂ɃN�����v�i�덷�΍�j
		dot = std::clamp(dot, -1.0, 1.0);

		// �p�x�v�Z
		return 2.0 * std::acos(dot);
	}

private:


	// ��{�x�N�g�����擾
	Vector3 GetDir(Vector3 dir) const;

};

