#pragma once

struct UV {
	float u = 0.0f, v = 0.0f;
};

struct Vector2
{
	float x = 0.0f, y = 0.0f;

	auto operator <=>(const Vector2&)const = default;

	Vector2 operator+(const Vector2& rval)const {
		return Vector2(x + rval.x, y + rval.y);
	}
	Vector2 operator-(const Vector2& rval)const {
		return Vector2(x - rval.x, y - rval.y);
	}
	Vector2 operator*(float scale)const {
		return Vector2(x * scale, y * scale);
	}
	Vector2 operator/(float div)const {
		return Vector2(x / div, y / div);
	}
	Vector2 operator*(Vector2 scale)const {
		return Vector2(x * scale.x, y * scale.y);
	}

	void operator+=(const Vector2& rval) {
		x += rval.x;
		y += rval.y;
	}
	void operator-=(const Vector2& rval) {
		x -= rval.x;
		y -= rval.y;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	void operator/=(float div) {
		x /= div;
		y /= div;
	}

	// �x�N�g���̒���(�m����)��Ԃ�
	float Length()const {
		return hypotf(x, y);
	}

	// �x�N�g���̒�����2���Ԃ�
	float LengthSquared()const {
		return x * x + y * y;
	}

	// ���K��
	void Normalize(void) {
		float len = Length();
		if (len == 0.0f) {
			x = 0.0f;
			y = 0.0f;
			return;
		}
		x /= len;
		y /= len;
	}

	// �����𐳋K�������x�N�g����Ԃ�
	Vector2 Normalized(void)const {
		float len = Length();
		if (len == 0.0f)return Vector2(0, 0);
		return Vector2(x / len, y / len);
	}
};

using Position2D = Vector2;
using Size2D = Vector2;
using Scale2D = Vector2;
using Direction2D = Vector2;

struct Vector3
{
	float x, y, z;

	auto operator <=>(const Vector3&)const = default;

	Vector3 operator+(const Vector3& rval)const {
		return Vector3(x + rval.x, y + rval.y, z + rval.z);
	}
	Vector3 operator-(const Vector3& rval)const {
		return Vector3(x - rval.x, y - rval.y, z - rval.z);
	}
	Vector3 operator*(float scale)const {
		return Vector3(x * scale, y * scale, z * scale);
	}
	Vector3 operator/(float div)const {
		return Vector3(x / div, y / div, z / div);
	}


	void operator+=(const Vector3& rval) {
		x += rval.x;
		y += rval.y;
		z += rval.z;
	}
	void operator-=(const Vector3& rval) {
		x -= rval.x;
		y -= rval.y;
		z -= rval.z;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	void operator/=(float div) {
		x /= div;
		y /= div;
		z /= div;
	}

	// �x�N�g���̒���(�m����)��Ԃ�
	float Length()const {
		return hypotf(hypotf(x, y), z);
	}

	// �x�N�g���̒�����2���Ԃ�
	float LengthSquared()const {
		return x * x + y * y + z * z;
	}

	// ���K��
	void Normalize(void) {
		float len = Length();
		if (len == 0.0f)
		{
			x = y = z = 0.0f;
			return;
		}
		x /= len;
		y /= len;
		z /= len;
	}

	// �����𐳋K�������x�N�g����Ԃ�
	Vector3 Normalized()const {
		float len = Length();
		if (len == 0.0f)
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}
		return Vector3(x / len, y / len, z / len);
	}

	Vector3 ClampEpsilon() const {
		float epsilon = 1e-4f;
		Vector3 ret = { x,y,z };
		if (fabs(x) < epsilon) ret.x = 0.0f;
		if (fabs(y) < epsilon) ret.y = 0.0f;
		if (fabs(z) < epsilon) ret.z = 0.0f;
		return ret;
	}
};

using Position3D = Vector3;
using Scale3D = Vector3;
using Direction3D = Vector3;

static inline Vector2 ToVector2(Vector3 vec3) {
	return Vector2(vec3.x, vec3.y);
}

static inline Vector3 ToVector3(Vector2 vec2) {
	return Vector3(vec2.x, vec2.y, 0.0f);
}

static inline float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline Vector3 Cross(const Vector3& vecA, const Vector3& vecB) {
	Vector3 Result;
	Result.x = vecA.y * vecB.z - vecA.z * vecB.y;
	Result.y = vecA.z * vecB.x - vecA.x * vecB.z;
	Result.z = vecA.x * vecB.y - vecA.y * vecB.x;
	return Result;
}

// �_���|���S���̕��ʂɓ��e����
static inline Vector3 ProjectPointOntoPlane(const Vector3& point, const Vector3& planePoint, const Vector3& planeNormal) {
	Vector3 toPoint = point - planePoint;
	float distance = Dot(toPoint,planeNormal);
	return point - planeNormal * distance;
}

// �_��3D�|���S�����ɂ��邩����i���ʏ�ł�2D����j
static inline bool IsPointInsidePolygon3D(const Vector3& P, const std::vector<Vector3>& vertices, const Vector3& normal) {
	size_t vertexCount = vertices.size();
	for (size_t i = 0; i < vertexCount; ++i) {
		Vector3 A = vertices[i];
		Vector3 B = vertices[(i + 1) % vertexCount];
		Vector3 edge = B - A;
		Vector3 toPoint = P - A;

		// �O�ς����ʖ@���Ɠ��������Ȃ����
		if (Dot(normal, Cross(edge,toPoint)) < 0) {
			return false;
		}
	}
	return true;
}

// ����AB��̍ŋߖT�_�����߂�
static inline Vector3 ClosestPointOnLine(const Vector3& P, const Vector3& A, const Vector3& B) {
	Vector3 AB = B - A;
	float t = (Dot((P - A),AB)) / Dot(AB,AB);
	t = clamp(t, 0.0f, 1.0f); // �������Ɍ���
	return A + AB * t;
}

// 3D�|���S�����ƃG�b�W����l�������ŋߖT�_�����߂�
static inline Vector3 ClosestPointOnPolygon(const Vector3& P, const std::vector<Vector3>& vertices) {
	if (vertices.size() < 3) {
		throw std::invalid_argument("The polygon must have at least three vertices.");
	}

	// �|���S���@�����v�Z
	Vector3 normal = Cross((vertices[1] - vertices[0]),(vertices[2] - vertices[0])).Normalized();

	// �_���|���S���̕��ʂɓ��e
	Vector3 projectedPoint = ProjectPointOntoPlane(P, vertices[0], normal);

	// �|���S����������
	if (IsPointInsidePolygon3D(projectedPoint, vertices, normal)) {
		return projectedPoint; // ���ʏ�ɂ���΂��̓_��Ԃ�
	}

	// �|���S���̃G�b�W��̍ŋߖT�_���v�Z
	Vector3 closestPoint;
	float closestDistanceSquared = (std::numeric_limits<float>::max)();

	for (size_t i = 0; i < vertices.size(); ++i) {
		Vector3 A = vertices[i];
		Vector3 B = vertices[(i + 1) % vertices.size()];
		Vector3 Q = ClosestPointOnLine(P, A, B);

		float distanceSquared = (P - Q).LengthSquared();
		if (distanceSquared < closestDistanceSquared) {
			closestDistanceSquared = distanceSquared;
			closestPoint = Q;
		}
	}

	return closestPoint;
}

// 3�̃x�N�g���̂����A���ƍł����Ă���x�N�g����I�Ԋ֐�
static inline Vector3 GetMostCommonNormal(const Vector3& normalA, const Vector3& normalB, const Vector3& normalC) {
	// ���K�����Ċp�x�v�Z���ȒP�ɂ���
	Vector3 nA = normalA.Normalized();
	Vector3 nB = normalB.Normalized();
	Vector3 nC = normalC.Normalized();

	// �h�b�g�ς��v�Z�i�ގ��x���m�F�j
	float dotAB = Dot(nA,nB);  // A��B�̗ގ��x
	float dotAC = Dot(nA,nC);  // A��C�̗ގ��x
	float dotBC = Dot(nB,nC);  // B��C�̗ގ��x

	// �ł����̃x�N�g���Ɨގ����Ă���x�N�g����Ԃ�
	if (dotAB > dotAC && dotAB > dotBC) {
		return normalA;  // A��B���ł��ގ����Ă���ꍇ
	}
	else if (dotAC > dotAB && dotAC > dotBC) {
		return normalA;  // A��C���ł��ގ����Ă���ꍇ
	}
	else {
		return normalB;  // B��C���ł��ގ����Ă���ꍇ
	}
}

static inline Vector3 FindNearestPointOnPlane(const Vector3& P, const Vector3& planePoint, const Vector3& planeNormal) {
	// ���ʂ̖@���𐳋K��
	Vector3 n = planeNormal.Normalized();

	// P���畽�ʂ܂ł̋������v�Z
	float distance = Dot(P, n) - Dot(planePoint, n);

	// �ŋߓ_�̌v�Z: P����@�������ɋ���������
	return {
		P.x - distance * n.x,
		P.y - distance * n.y,
		P.z - distance * n.z
	};
}

enum class DIR
{
	UP,
	LEFT,
	DOWN,
	RIGHT,
	MAX
};


static inline wstring cp_to_wide(const string& s, UINT codepage = 932)
{
	int in_length = (int)s.length();
	int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
	wstring result(out_length, L'\0');
	if (out_length) MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0], out_length);
	return result;
}

static inline string wide_to_cp(const wstring& s, UINT codepage = 932)
{
	int in_length = (int)s.length();
	int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
	string result(out_length, '\0');
	if (out_length) WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0], out_length, 0, 0);
	return result;
}

// 3*3�s��
struct Matrix {
	float m[3][3];
};

///�Q�̍s��̏�Z��Ԃ�
///@param lmat ���Ӓl(�s��)
///@param rmat �E�Ӓl(�s��)
///@attention ��Z�̏����ɒ��ӂ��Ă�������
static inline Matrix
	MultipleMat(const Matrix& lmat, const Matrix& rmat) {
	Matrix ret = {};

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				ret.m[y][x] += lmat.m[y][i] * rmat.m[i][x];
			}
		}
	}

	return ret;

}

static inline Matrix operator*(const Matrix& lmat, const Matrix& rmat)
{
	return MultipleMat(lmat, rmat);
}



///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
static inline Vector2
	MultipleVec(const Matrix& mat, const Vector2& vec) {
	Vector2 ret = {};

	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * 1.0f;
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * 1.0f;

	return ret;
}

static inline Vector2 operator*(const Matrix& mat, const Vector2& vec)
{
	return MultipleVec(mat, vec);
}

///�P�ʍs���Ԃ�
static inline Matrix IdentityMat() {
	Matrix ret = {
		{
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f}
		}
	};

	return ret;
}

///���s�ړ��s���Ԃ�
///@param x X�������s�ړ���
///@param y Y�������s�ړ���
static inline Matrix TranslateMat(float x, float y) {
	Matrix ret = {
		{
			{1.0f,0.0f,x},
			{0.0f,1.0f,y},
			{0.0f,0.0f,1.0f}
		}
	};

	return ret;
}

///��]�s���Ԃ�
///@param angle ��]�p�x
static inline Matrix RotateMat(float angle) {
	Matrix ret = {
		{
			{cosf(angle),-sinf(angle),0.0f},
			{sinf(angle), cosf(angle),0.0f},
			{0.0f,0.0f,1.0f}
		}
	};
	return ret;
}

///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ�
///@param center ��]���S�_
///@param angle ��]�p�x
///@param pos ���̍��W
///@return �ϊ���̍��W
static inline Vector2 RotatePosition(const Vector2& center, float angle, const Vector2& pos) {
	Matrix mat = TranslateMat(center.x, center.y)
		* RotateMat(angle)
		* TranslateMat(-center.x, -center.y);

	auto ret = mat * pos;
	return Vector2(ret.x, ret.y);
}

static inline Matrix RotateMatrix(const Vector2& center, float angle) {
	return TranslateMat(center.x, center.y)
		* RotateMat(angle)
		* TranslateMat(-center.x, -center.y);
}

template <typename T>
concept LerpPossibleType =
	is_same_v<T, int>		||
	is_same_v<T, float>		||
	is_same_v<T, double>	||
	is_same_v<T, Vector2>	||
	is_same_v<T, Vector3>;

template <LerpPossibleType T>
static inline T Lerp(T start, T end, float rate)
{
	return start + ((end - start) * rate);
}



template <typename T>
concept AnglePossibleType =
	is_same_v<T, int> ||
	is_same_v<T, float> ||
	is_same_v<T, double>;

template <AnglePossibleType T>
static inline T Radian2Deg(const T& radian)
{
	return radian * static_cast<T>(180) / static_cast<T>(M_PI);
}

template <AnglePossibleType T>
static inline T Deg2Radian(const T& deg)
{
	return deg * static_cast<T>(M_PI) / static_cast<T>(180);
}

template <AnglePossibleType T>
static inline T DegIn360(T deg)
{
	float ret = static_cast<float>(deg);
	ret = fmodf(ret, 360.0f);
	if (ret < 0.0f)
	{
		ret += 360.0f;
	}
	return static_cast<T>(ret);
}

template <AnglePossibleType T>
static inline T DegIn180(T deg)
{
	float ret = static_cast<float>(deg);
	ret = fmodf(ret, 180.0f);
	if (ret < 0.0f)
	{
		ret += 180.0f;
	}
	return static_cast<T>(ret);
}

template <AnglePossibleType T>
static inline T RadIn2PI(T rad)
{
	float ret = static_cast<float>(rad);
	ret = fmodf(ret, static_cast<float>(M_PI) * 2.0f);
	if (ret < 0.0f)
	{
		ret += static_cast<float>(M_PI) * 2.0f;
	}
	return static_cast<T>(ret);
}

template <AnglePossibleType T>
static inline T RadInPI(T rad)
{
	float ret = static_cast<float>(rad);
	ret = fmodf(ret, static_cast<float>(M_PI));
	if (ret < 0.0f)
	{
		ret += static_cast<float>(M_PI);
	}
	return static_cast<T>(ret);
}

static inline int DirNearAroundRad(float from, float to)
{
	int ret = 1;

	float diff = to - from;

	if (diff >= 0.0f) {
		// ��r���������v���Ɉʒu����
		if (diff > M_PI) {
			// �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
			ret = -1;
		}
		else {
			// ���v���
			ret = 1;
		}
	}
	else {
		// ��r�����������v���Ɉʒu����
		if (diff < -M_PI) {
			// �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
			ret = 1;
		}
		else {
			// �����v���
			ret = -1;
		}
	}

	return ret;
}

// ������̕���
static inline vector<string> Split(string line, char delmiter)
{
	// �ϒ��z��̕Ԃ�l������
	vector<string> result;

	// string���t�@�C���X�g���[���ɕϊ�
	istringstream stream(line);

	// �ǂݍ��񂾕�������i�[����p�ϐ�
	string field;

	// stream�����Ԃɓǂݍ���
	// ��3�����Ŏw�肳�ꂽ�����܂ł��2�����ɕԂ�
	while (getline(stream, field, delmiter))
	{
		// �ϒ��z��ɒǉ�
		result.push_back(field);
	}

	return result;
}

static random_device rd;
static mt19937 gen(rd());
static inline int GetRand()
{
	return gen();
}

/// @brief �������擾(min�ȏ�,max����)
/// @param min �ŏ�
/// @param max �ő�
template<typename T>
static inline T GetRand(T min, T max)
{
	uniform_real_distribution<> urd(min, max);
	return static_cast<T>(urd(gen));
}

// ����
static inline float Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// �O��
static inline float Cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

static inline float CalculateAngle(const Vector3& a, const Vector3& b) {
	float dotProduct = Dot(a,b);
	float lengths = a.Length() * b.Length();

	if (lengths == 0.0f) {
		return 0.0f;
	}

	float cosTheta = dotProduct / lengths;

	// �덷�̕␳
	cosTheta = max(-1.0f, min(1.0f, cosTheta));

	// �p�x���v�Z�i���W�A���j
	return acos(cosTheta);
}

// �������m�̏Փ˔���
static inline Vector2 CollisionLines(const Vector2& lineAPos1, const Vector2& lineAPos2, const Vector2& lineBPos1, const Vector2& lineBPos2)
{
	auto deno = Cross(lineAPos2 - lineAPos1, lineBPos2 - lineBPos1);
	auto s = deno;
	auto t = deno;

	auto error = Vector2(0.0f, 0.0f);
	if (deno == 0.0f) {
		// ���������s
		return error;
	}


	s = Cross(lineBPos1 - lineAPos1, lineBPos2 - lineBPos1) / deno;
	t = Cross(lineAPos2 - lineAPos1, lineAPos1 - lineBPos1) / deno;
	if (s < 0.0f || 1.0f < s || t < 0.0f || 1.0f < t) {
		// �������������Ă��Ȃ�
		return error;
	}

	auto v = lineAPos2 - lineAPos1;

	return Vector2(lineAPos1.x + s * v.x, lineAPos1.y + s * v.y);
}

// ����A (A1, A2) �Ɛ���B (B1, B2) �̌�������
static inline bool DoLinesIntersect(const Position3D& A1, const Position3D& A2, const Position3D& B1, const Position3D& B2) {
	// ����A�Ɛ���B�̕����x�N�g��
	Vector3 d1 = A2 - A1;
	Vector3 d2 = B2 - B1;
	Vector3 r = B1 - A1;

	// d1��d2�̊O��
	Vector3 cross1 = Cross(d1, d2);

	// �O�ς̑傫����2�� (�[���̏ꍇ�͕��s�܂��͓������)
	float denom = Dot(cross1, cross1);
	if (denom == 0.0f) {
		return false; // ���������s�܂��͓���Ō������Ȃ�
	}

	// ����A��̈ʒu t1, ����B��̈ʒu t2 ���v�Z
	float t1 = Dot(Cross(r, d2), cross1) / denom;
	float t2 = Dot(Cross(r, d1), cross1) / denom;

	// t1, t2 �� [0, 1] �͈͓̔��ɂ���ꍇ�A�����͌���
	return (t1 >= 0.0f && t1 <= 1.0f) && (t2 >= 0.0f && t2 <= 1.0f);
}

static inline bool IsPointOnSegment(const Position3D& point, const Position3D& segStart, const Position3D& segEnd) {
	Vector3 seg = segEnd - segStart;      // �����̃x�N�g��
	Vector3 toPoint = point - segStart;  // �n�_����_�ւ̃x�N�g��

	// �_��������ɂ��邩�i�O�ς��[���ł��邩���m�F�j
	Vector3 crossProduct = Cross(seg, toPoint);
	if (crossProduct.LengthSquared() > 1e-6f) { // ���e�덷
		return false; // ������ɂȂ�
	}

	// �_�������͈͓̔��ɂ��邩�i�ˉe�l t �� 0 ���� 1 �͈̔͂ɂ��邩�j
	float dotProduct = Dot(toPoint, seg);
	if (dotProduct < 0.0f || dotProduct > seg.LengthSquared()) {
		return false; // �͈͊O
	}

	return true; // ������ɂ���
}

// �_����`�̒��ɓ��荞��ł��邩
static inline bool CollisionPointInBox(const Vector2& pointPos, const Vector2& boxpos, const Vector2& boxSize)
{
	Vector2 LTpos = { boxpos.x - boxSize.x / 2.0f,boxpos.y - boxSize.y / 2.0f };// ����
	Vector2 RDpos = { boxpos.x + boxSize.x / 2.0f,boxpos.y + boxSize.y / 2.0f };// �E��

	if (LTpos.x <= pointPos.x && pointPos.x <= RDpos.x &&
		LTpos.y <= pointPos.y && pointPos.y <= RDpos.y) {
		return true;
	}

	return false;
}

// �_�Ƌ�`(��]����)���������Ă��邩�ǂ���
// @param point �_
// @param box ��`(��]����)
static inline bool CollisionPointRotateBox(const Vector2& pointPos, const Vector2 boxPos[4])
{

	Vector2 V1to2 = (boxPos[1] - boxPos[0]);
	Vector2 V2to3 = (boxPos[2] - boxPos[1]);
	Vector2 V3to4 = (boxPos[3] - boxPos[2]);
	Vector2 V4to1 = (boxPos[0] - boxPos[3]);

	// ��`��4�ӂƁA�ӂ̎n�_����_�Ɍ������x�N�g���̃Z�b�g
	vector<pair<Vector2, Vector2>> datas = {
		{ V1to2, (pointPos - boxPos[0]) },
		{ V2to3, (pointPos - boxPos[1]) },
		{ V3to4, (pointPos - boxPos[2]) },
		{ V4to1, (pointPos - boxPos[3]) }
	};

	// ����������
	// ��`�̊e�ӂ�v1�A�ӂ̎n�_����_�Ɍ������x�N�g����v2�Ƃ���
	// v1��v2�̊O�ς����̒l�������ꍇ�͂��̎��_�ŋ�`�͈̔͊O�ɂȂ�̂Ŕ�����I������
	// 4�ӑS�Ăɑ΂��A�O�ς̌��ʂ��}�C�i�X�������ꍇ�͋�`�̒��ɓ_������
	for (auto& data : datas)
	{
		auto cross = Cross(data.first, data.second);
		if (0 < cross) return false;
	}

	return true;
}

/// @brief �������藝��p����2D�O�p�`���m�̏Փ˔���
/// @param triA ��ڂ̎O�p�`
/// @param triB ��ڂ̎O�p�`
/// @return �Փ˂��Ă���ꍇ��true�A����ȊO��false
static inline bool CheckTriangleCollision2D(const Vector2* triA, const Vector2* triB) {
	auto GetAxes = [](const Vector2* tri) -> vector<Vector2> {
		std::vector<Vector2> axes;
		for (int i = 0; i < 3; ++i) {
			Vector2 edge = tri[(i + 1) % 3] - tri[i];
			axes.push_back(Vector2( -edge.y, edge.x).Normalized()); // �G�b�W�ɐ����Ȗ@��
		}
		return axes;
		};

	auto ProjectTriangle = [](const Vector2* tri, const Vector2& axis) -> std::pair<float, float> {
		float minProj = (numeric_limits<float>::max)();
		float maxProj = numeric_limits<float>::lowest();

		for (int i = 0; i < 3; i++) {
			float proj = Dot(tri[i],axis);
			minProj = min(minProj, proj);
			maxProj = max(maxProj, proj);
		}
		return { minProj, maxProj };
		};

	auto CheckOverlap = [](const std::pair<float, float>& rangeA, const std::pair<float, float>& rangeB) -> bool {
		return !(rangeA.second < rangeB.first || rangeB.second < rangeA.first);
		};

	// �����擾
	auto axesA = GetAxes(triA);
	auto axesB = GetAxes(triB);

	// A�̎��ŕ��������m�F
	for (const auto& axis : axesA) {
		auto rangeA = ProjectTriangle(triA, axis);
		auto rangeB = ProjectTriangle(triB, axis);
		if (!CheckOverlap(rangeA, rangeB)) {
			return false; // ������������
		}
	}

	// B�̎��ŕ��������m�F
	for (const auto& axis : axesB) {
		auto rangeA = ProjectTriangle(triA, axis);
		auto rangeB = ProjectTriangle(triB, axis);
		if (!CheckOverlap(rangeA, rangeB)) {
			return false; // ������������
		}
	}

	return true; // �Փ˂��Ă���
}


/// @brief �ʕ�����߂�֐�
/// @param points ���͓_�W��
/// @return �ʕ�̒��_���X�g
static inline vector<Vector2> ComputeConvexHull(std::vector<Vector2> points) {
	std::sort(points.begin(), points.end()); // x, y �̏��Ń\�[�g

	// �����ʕ���\�z
	std::vector<Vector2> lower;
	for (const auto& p : points) {
		while (lower.size() >= 2) {
			Vector2 q = lower[lower.size() - 2];
			Vector2 r = lower[lower.size() - 1];
			if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0) { // ���v���܂��͒���
				lower.pop_back();
			}
			else {
				break;
			}
		}
		lower.push_back(p);
	}

	// �㕔�ʕ���\�z
	std::vector<Vector2> upper;
	for (auto it = points.rbegin(); it != points.rend(); ++it) {
		const auto& p = *it;
		while (upper.size() >= 2) {
			Vector2 q = upper[upper.size() - 2];
			Vector2 r = upper[upper.size() - 1];
			if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0) { // ���v���܂��͒���
				upper.pop_back();
			}
			else {
				break;
			}
		}
		upper.push_back(p);
	}

	// �㕔�Ɖ����𓝍��i�ŏ��ƍŌ�̓_���d�����Ă���̂ō폜�j
	lower.pop_back();
	upper.pop_back();
	lower.insert(lower.end(), upper.begin(), upper.end());
	return lower;
}

/// @brief �ʕ��2�̎O�p�`�ɕ�������
/// @param hull �ʕ�̒��_���X�g
/// @return 2�̎O�p�`�ɕ�����������
static inline std::vector<std::vector<Vector2>> SplitIntoTriangles(const std::vector<Vector2>& hull) {
	std::vector<std::vector<Vector2>> triangles;
	if (hull.size() < 4) {
		throw std::runtime_error("�ʕ�ɏ��Ȃ��Ƃ�4�_���K�v�ł��B");
	}

	// ��_��I�сA��`�ɎO�p�`�����
	const auto& base = hull[0];
	for (size_t i = 1; i < hull.size() - 1; ++i) {
		triangles.push_back({ base, hull[i], hull[i + 1] });
	}
	return triangles;
}

/// @brief 2�̎O�p�`�|���S��������l�p�`���\������2�̎O�p�`�|���S���ɕϊ�����֐�
/// @param triA ��ڂ̎O�p�`
/// @param triB ��ڂ̎O�p�`
/// @return �l�p�`���\������2�̎O�p�`
static inline std::vector<std::vector<Vector2>> ConvertTrianglesToBoundingTriangles(
	const std::vector<Vector2>& triA,
	const std::vector<Vector2>& triB) {

	if (triA.size() != 3 || triB.size() != 3) {
		throw std::invalid_argument("�O�p�`��3�_�ō\�������K�v������܂��B");
	}

	// ���_���܂Ƃ߂�
	std::vector<Vector2> points = triA;
	points.insert(points.end(), triB.begin(), triB.end());

	// �ʕ���v�Z
	std::vector<Vector2> hull = ComputeConvexHull(points);

	// �ʕ��2�̎O�p�`�ɕ���
	return SplitIntoTriangles(hull);
}

static inline bool CollisionBox(Position2D boxApos, Size2D boxASize, Position2D boxBpos, Size2D boxBSize) {

	return !(abs(boxApos.x - boxBpos.x) >= (boxASize.x / 2.0f + boxBSize.x / 2.0f) ||
		abs(boxApos.y - boxBpos.y) >= (boxASize.y / 2.0f + boxBSize.y / 2.0f));
}

// �w�肵�����̋�`�Ƌ�`�̂߂荞�ݗʂ�Ԃ�
static inline float GetPenetrationDepth(int isX, Position2D boxApos, Size2D boxASize, Position2D boxBpos, Size2D boxBSize) {

	float ret = 0.0f;

	if (abs(boxApos.x - boxBpos.x) >= (boxASize.x / 2.0f + boxBSize.x / 2.0f) ||
		abs(boxApos.y - boxBpos.y) >= (boxASize.y / 2.0f + boxBSize.y / 2.0f)) return ret;

	if (isX) {
		ret = (boxASize.x / 2.0f + boxBSize.x / 2.0f) - abs(boxApos.x - boxBpos.x);
	}
	else {
		ret = (boxASize.y / 2.0f + boxBSize.y / 2.0f) - abs(boxApos.y - boxBpos.y);
	}

	return ret;
}

// �~�Ɖ~�̏Փ˔���
static inline bool IsCollisionCircle(Position2D pos1, float radi1, Position2D pos2, float radi2)
{
	Position2D pos = { pos2.x - pos1.x,pos2.y - pos1.y };
	float dis = sqrt(pos.x * pos.x + pos.y * pos.y);
	float rad = radi1 + radi2;

	// ���S��������̉~�̔��a�𑫂������̂����Z����Γ������Ă���
	if (dis < rad)
	{
		return true;
	}

	return false;
}

/// @brief ��`�Ɖ~�̏Փ˔���
/// @param rectCenter  
/// @param rectSize
/// @param circleCenter 
/// @param radius 
/// @return 
static inline bool IsCollisionRect2Circle(Position2D rectCenter, Size2D rectSize, Position2D circleCenter, float radius)
{
	Size2D size = { radius * 2.0f ,radius * 2.0f };
	if (!CollisionBox(rectCenter, rectSize, circleCenter, size)) return false;

	// �̈�A�Փ˔���
	Position2D posA1 = { rectCenter.x - (rectSize.x / 2),rectCenter.y - (rectSize.y / 2) - radius };
	Position2D posA2 = { rectCenter.x + (rectSize.x / 2),rectCenter.y + (rectSize.y / 2) + radius };
	if (posA1.x <= circleCenter.x && circleCenter.x <= posA2.x &&
		posA1.y <= circleCenter.y && circleCenter.y <= posA2.y)
	{
		return true;
	}

	// �̈�B�Փ˔���
	Position2D posB1 = { rectCenter.x - (rectSize.x / 2) - radius,rectCenter.y - (rectSize.y / 2) };
	Position2D posB2 = { rectCenter.x + (rectSize.x / 2) + radius,rectCenter.y + (rectSize.y / 2) };
	if (posB1.x <= circleCenter.x && circleCenter.x <= posB2.x &&
		posB1.y <= circleCenter.y && circleCenter.y <= posB2.y)
	{
		return true;
	}

	// �̈�C�Փ˔���
	Position2D posC = { rectCenter.x - (rectSize.x / 2) ,rectCenter.y - (rectSize.y / 2) };
	if (IsCollisionCircle(posC, radius, circleCenter, radius)) return true;

	// �̈�D�Փ˔���
	Position2D posD = { rectCenter.x + (rectSize.x / 2) ,rectCenter.y - (rectSize.y / 2) };
	if (IsCollisionCircle(posD, radius, circleCenter, radius)) return true;

	// �̈�E�Փ˔���
	Position2D posE = { rectCenter.x - (rectSize.x / 2) ,rectCenter.y + (rectSize.y / 2) };
	if (IsCollisionCircle(posE, radius, circleCenter, radius)) return true;

	// �̈�F�Փ˔���
	Position2D posF = { rectCenter.x + (rectSize.x / 2) ,rectCenter.y + (rectSize.y / 2) };
	if (IsCollisionCircle(posF, radius, circleCenter, radius)) return true;


	return false;
}

static inline void LoadCsvData(string filePath, vector<int>& mapVec, int& width, int& height)
{
	mapVec.clear();
	width = 0;
	height = 0;

	// �t�@�C���̃p�X
	string path = filePath;

	// �t�@�C���̓ǂݍ���
	ifstream ifs = ifstream(path.c_str());
	if (!ifs)
	{
		// �G���[������
		return;
	}

	// �t�@�C����1�s���ǂݍ���ŁAline�Ɋi�[����
	string line;					// 1�s�̕������
	vector<string> split;		// 1����1�����̕�������
	int chipNo = 0;

	// �t�@�C����1�s���ǂݍ���
	while (getline(ifs, line))
	{
		// 1�s���̕�������J���}��؂�ŕ���
		split = Split(line, ',');

		width = (int)split.size();

		for (int x = 0; x < split.size(); x++)
		{

			// string�^����int�^�ɕϊ�
			int chipNo = stoi(split[x]);

			// �z��Ƀ}�b�v�`�b�v�ԍ����i�[
			mapVec.emplace_back(chipNo);
		}
		height++;
	}
}

static inline string GetExtension(const string& filePath)
{
	size_t length = filePath.find_last_of(".");

	return filePath.substr(length + 1);
}

static inline XMFLOAT3 GetTranslationFromMatrix(const XMMATRIX& matrix)
{
	return XMFLOAT3(matrix.r[3].m128_f32[0],  // m30
					matrix.r[3].m128_f32[1],  // m31
					matrix.r[3].m128_f32[2]); // m32
}

static inline XMFLOAT3 GetScaleFromMatrix(const XMMATRIX& matrix)
{
	XMFLOAT3 scale;
	scale.x = XMVectorGetX(XMVector3Length(matrix.r[0]));  // X���̃X�P�[��
	scale.y = XMVectorGetY(XMVector3Length(matrix.r[1]));  // Y���̃X�P�[��
	scale.z = XMVectorGetZ(XMVector3Length(matrix.r[2]));  // Z���̃X�P�[��
	return scale;
}

static inline XMMATRIX GetRotationMatrixFromMatrix(const XMMATRIX& matrix)
{
	// �e���x�N�g���iX, Y, Z�j�𐳋K�����ăX�P�[������菜��
	XMVECTOR xAxis = XMVector3Normalize(matrix.r[0]); // X��
	XMVECTOR yAxis = XMVector3Normalize(matrix.r[1]); // Y��
	XMVECTOR zAxis = XMVector3Normalize(matrix.r[2]); // Z��

	// ��]�����݂̂̍s����\�z
	return XMMATRIX(xAxis, yAxis, zAxis, XMVectorSet(0, 0, 0, 1));
}

static inline void GetEulerAnglesFromMatrix(const XMMATRIX& mat, float& roll, float& pitch, float& yaw) {
	// ��]�s��̗v�f���擾
	float m00 = mat.r[0].m128_f32[0];
	float m01 = mat.r[0].m128_f32[1];
	float m02 = mat.r[0].m128_f32[2];
	float m10 = mat.r[1].m128_f32[0];
	float m11 = mat.r[1].m128_f32[1];
	float m12 = mat.r[1].m128_f32[2];
	float m20 = mat.r[2].m128_f32[0];
	float m21 = mat.r[2].m128_f32[1];
	float m22 = mat.r[2].m128_f32[2];

	// �s�b�`�iY�����j���擾
	pitch = std::asin(-m20);

	// �K���}�s��̓��ْl�i�s�b�`�p���}90�x�j���l��
	if (std::abs(m20) < 0.9999f) {
		// �s�b�`�p���ʏ�͈͓��̏ꍇ
		yaw = std::atan2(m10, m00);
		roll = std::atan2(m21, m22);
	}
	else {
		// �s�b�`�p���}90�x�̓��ْl�̏ꍇ
		yaw = std::atan2(-m01, m11);
		roll = 0.0f;
	}
}


/// @brief ���\�[�X�o���A���Z�b�g
/// @param Resource �ύX���郊�\�[�X
/// @param before �ύX�O
/// @param after �ύX��
static inline void SetResourceBarrier(ID3D12Resource* Resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = Resource;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = before;
	DxCommand->ResourceBarrier(1, &barrier);
}

#define SCREEN_CENTER Vector2(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f)

#include "Timer/Timer.h"
