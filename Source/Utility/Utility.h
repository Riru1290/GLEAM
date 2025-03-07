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

	// ベクトルの長さ(ノルム)を返す
	float Length()const {
		return hypotf(x, y);
	}

	// ベクトルの長さの2乗を返す
	float LengthSquared()const {
		return x * x + y * y;
	}

	// 正規化
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

	// 自分を正規化したベクトルを返す
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

	// ベクトルの長さ(ノルム)を返す
	float Length()const {
		return hypotf(hypotf(x, y), z);
	}

	// ベクトルの長さの2乗を返す
	float LengthSquared()const {
		return x * x + y * y + z * z;
	}

	// 正規化
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

	// 自分を正規化したベクトルを返す
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

// 点をポリゴンの平面に投影する
static inline Vector3 ProjectPointOntoPlane(const Vector3& point, const Vector3& planePoint, const Vector3& planeNormal) {
	Vector3 toPoint = point - planePoint;
	float distance = Dot(toPoint,planeNormal);
	return point - planeNormal * distance;
}

// 点が3Dポリゴン内にあるか判定（平面上での2D判定）
static inline bool IsPointInsidePolygon3D(const Vector3& P, const std::vector<Vector3>& vertices, const Vector3& normal) {
	size_t vertexCount = vertices.size();
	for (size_t i = 0; i < vertexCount; ++i) {
		Vector3 A = vertices[i];
		Vector3 B = vertices[(i + 1) % vertexCount];
		Vector3 edge = B - A;
		Vector3 toPoint = P - A;

		// 外積が平面法線と同じ向きなら内部
		if (Dot(normal, Cross(edge,toPoint)) < 0) {
			return false;
		}
	}
	return true;
}

// 線分AB上の最近傍点を求める
static inline Vector3 ClosestPointOnLine(const Vector3& P, const Vector3& A, const Vector3& B) {
	Vector3 AB = B - A;
	float t = (Dot((P - A),AB)) / Dot(AB,AB);
	t = clamp(t, 0.0f, 1.0f); // 線分内に限定
	return A + AB * t;
}

// 3Dポリゴン内とエッジ上を考慮した最近傍点を求める
static inline Vector3 ClosestPointOnPolygon(const Vector3& P, const std::vector<Vector3>& vertices) {
	if (vertices.size() < 3) {
		throw std::invalid_argument("The polygon must have at least three vertices.");
	}

	// ポリゴン法線を計算
	Vector3 normal = Cross((vertices[1] - vertices[0]),(vertices[2] - vertices[0])).Normalized();

	// 点をポリゴンの平面に投影
	Vector3 projectedPoint = ProjectPointOntoPlane(P, vertices[0], normal);

	// ポリゴン内部判定
	if (IsPointInsidePolygon3D(projectedPoint, vertices, normal)) {
		return projectedPoint; // 平面上にあればその点を返す
	}

	// ポリゴンのエッジ上の最近傍点を計算
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

// 3つのベクトルのうち、他と最も似ているベクトルを選ぶ関数
static inline Vector3 GetMostCommonNormal(const Vector3& normalA, const Vector3& normalB, const Vector3& normalC) {
	// 正規化して角度計算を簡単にする
	Vector3 nA = normalA.Normalized();
	Vector3 nB = normalB.Normalized();
	Vector3 nC = normalC.Normalized();

	// ドット積を計算（類似度を確認）
	float dotAB = Dot(nA,nB);  // AとBの類似度
	float dotAC = Dot(nA,nC);  // AとCの類似度
	float dotBC = Dot(nB,nC);  // BとCの類似度

	// 最も他のベクトルと類似しているベクトルを返す
	if (dotAB > dotAC && dotAB > dotBC) {
		return normalA;  // AとBが最も類似している場合
	}
	else if (dotAC > dotAB && dotAC > dotBC) {
		return normalA;  // AとCが最も類似している場合
	}
	else {
		return normalB;  // BとCが最も類似している場合
	}
}

static inline Vector3 FindNearestPointOnPlane(const Vector3& P, const Vector3& planePoint, const Vector3& planeNormal) {
	// 平面の法線を正規化
	Vector3 n = planeNormal.Normalized();

	// Pから平面までの距離を計算
	float distance = Dot(P, n) - Dot(planePoint, n);

	// 最近点の計算: Pから法線方向に距離を引く
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

// 3*3行列
struct Matrix {
	float m[3][3];
};

///２つの行列の乗算を返す
///@param lmat 左辺値(行列)
///@param rmat 右辺値(行列)
///@attention 乗算の順序に注意してください
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



///ベクトルに対して行列乗算を適用し、結果のベクトルを返す
///@param mat 行列
///@param vec ベクトル
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

///単位行列を返す
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

///平行移動行列を返す
///@param x X方向平行移動量
///@param y Y方向平行移動量
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

///回転行列を返す
///@param angle 回転角度
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

///とある点を中心に回転して、その回転後の座標を返す
///@param center 回転中心点
///@param angle 回転角度
///@param pos 元の座標
///@return 変換後の座標
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
		// 比較元よりも時計回りに位置する
		if (diff > M_PI) {
			// でも、180度以上離れているので、反時計回りの方が近い
			ret = -1;
		}
		else {
			// 時計回り
			ret = 1;
		}
	}
	else {
		// 比較元よりも反時計回りに位置する
		if (diff < -M_PI) {
			// でも、180度以上離れているので、時計回りの方が近い
			ret = 1;
		}
		else {
			// 反時計回り
			ret = -1;
		}
	}

	return ret;
}

// 文字列の分割
static inline vector<string> Split(string line, char delmiter)
{
	// 可変長配列の返り値を準備
	vector<string> result;

	// stringをファイルストリームに変換
	istringstream stream(line);

	// 読み込んだ文字列を格納する用変数
	string field;

	// streamを順番に読み込み
	// 第3引数で指定された文字までを第2引数に返す
	while (getline(stream, field, delmiter))
	{
		// 可変長配列に追加
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

/// @brief 乱数を取得(min以上,max未満)
/// @param min 最小
/// @param max 最大
template<typename T>
static inline T GetRand(T min, T max)
{
	uniform_real_distribution<> urd(min, max);
	return static_cast<T>(urd(gen));
}

// 内積
static inline float Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// 外積
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

	// 誤差の補正
	cosTheta = max(-1.0f, min(1.0f, cosTheta));

	// 角度を計算（ラジアン）
	return acos(cosTheta);
}

// 線分同士の衝突判定
static inline Vector2 CollisionLines(const Vector2& lineAPos1, const Vector2& lineAPos2, const Vector2& lineBPos1, const Vector2& lineBPos2)
{
	auto deno = Cross(lineAPos2 - lineAPos1, lineBPos2 - lineBPos1);
	auto s = deno;
	auto t = deno;

	auto error = Vector2(0.0f, 0.0f);
	if (deno == 0.0f) {
		// 線分が平行
		return error;
	}


	s = Cross(lineBPos1 - lineAPos1, lineBPos2 - lineBPos1) / deno;
	t = Cross(lineAPos2 - lineAPos1, lineAPos1 - lineBPos1) / deno;
	if (s < 0.0f || 1.0f < s || t < 0.0f || 1.0f < t) {
		// 線分が交差していない
		return error;
	}

	auto v = lineAPos2 - lineAPos1;

	return Vector2(lineAPos1.x + s * v.x, lineAPos1.y + s * v.y);
}

// 線分A (A1, A2) と線分B (B1, B2) の交差判定
static inline bool DoLinesIntersect(const Position3D& A1, const Position3D& A2, const Position3D& B1, const Position3D& B2) {
	// 線分Aと線分Bの方向ベクトル
	Vector3 d1 = A2 - A1;
	Vector3 d2 = B2 - B1;
	Vector3 r = B1 - A1;

	// d1とd2の外積
	Vector3 cross1 = Cross(d1, d2);

	// 外積の大きさの2乗 (ゼロの場合は平行または同一線上)
	float denom = Dot(cross1, cross1);
	if (denom == 0.0f) {
		return false; // 線分が平行または同一で交差しない
	}

	// 線分A上の位置 t1, 線分B上の位置 t2 を計算
	float t1 = Dot(Cross(r, d2), cross1) / denom;
	float t2 = Dot(Cross(r, d1), cross1) / denom;

	// t1, t2 が [0, 1] の範囲内にある場合、線分は交差
	return (t1 >= 0.0f && t1 <= 1.0f) && (t2 >= 0.0f && t2 <= 1.0f);
}

static inline bool IsPointOnSegment(const Position3D& point, const Position3D& segStart, const Position3D& segEnd) {
	Vector3 seg = segEnd - segStart;      // 線分のベクトル
	Vector3 toPoint = point - segStart;  // 始点から点へのベクトル

	// 点が直線上にあるか（外積がゼロであるかを確認）
	Vector3 crossProduct = Cross(seg, toPoint);
	if (crossProduct.LengthSquared() > 1e-6f) { // 許容誤差
		return false; // 直線上にない
	}

	// 点が線分の範囲内にあるか（射影値 t が 0 から 1 の範囲にあるか）
	float dotProduct = Dot(toPoint, seg);
	if (dotProduct < 0.0f || dotProduct > seg.LengthSquared()) {
		return false; // 範囲外
	}

	return true; // 線分上にある
}

// 点が矩形の中に入り込んでいるか
static inline bool CollisionPointInBox(const Vector2& pointPos, const Vector2& boxpos, const Vector2& boxSize)
{
	Vector2 LTpos = { boxpos.x - boxSize.x / 2.0f,boxpos.y - boxSize.y / 2.0f };// 左上
	Vector2 RDpos = { boxpos.x + boxSize.x / 2.0f,boxpos.y + boxSize.y / 2.0f };// 右上

	if (LTpos.x <= pointPos.x && pointPos.x <= RDpos.x &&
		LTpos.y <= pointPos.y && pointPos.y <= RDpos.y) {
		return true;
	}

	return false;
}

// 点と矩形(回転あり)が当たっているかどうか
// @param point 点
// @param box 矩形(回転あり)
static inline bool CollisionPointRotateBox(const Vector2& pointPos, const Vector2 boxPos[4])
{

	Vector2 V1to2 = (boxPos[1] - boxPos[0]);
	Vector2 V2to3 = (boxPos[2] - boxPos[1]);
	Vector2 V3to4 = (boxPos[3] - boxPos[2]);
	Vector2 V4to1 = (boxPos[0] - boxPos[3]);

	// 矩形の4辺と、辺の始点から点に向かうベクトルのセット
	vector<pair<Vector2, Vector2>> datas = {
		{ V1to2, (pointPos - boxPos[0]) },
		{ V2to3, (pointPos - boxPos[1]) },
		{ V3to4, (pointPos - boxPos[2]) },
		{ V4to1, (pointPos - boxPos[3]) }
	};

	// 分離軸判定
	// 矩形の各辺をv1、辺の始点から点に向かうベクトルをv2として
	// v1とv2の外積が正の値だった場合はその時点で矩形の範囲外になるので判定を終了する
	// 4辺全てに対し、外積の結果がマイナスだった場合は矩形の中に点がある
	for (auto& data : datas)
	{
		auto cross = Cross(data.first, data.second);
		if (0 < cross) return false;
	}

	return true;
}

/// @brief 分離軸定理を用いた2D三角形同士の衝突判定
/// @param triA 一つ目の三角形
/// @param triB 二つ目の三角形
/// @return 衝突している場合はtrue、それ以外はfalse
static inline bool CheckTriangleCollision2D(const Vector2* triA, const Vector2* triB) {
	auto GetAxes = [](const Vector2* tri) -> vector<Vector2> {
		std::vector<Vector2> axes;
		for (int i = 0; i < 3; ++i) {
			Vector2 edge = tri[(i + 1) % 3] - tri[i];
			axes.push_back(Vector2( -edge.y, edge.x).Normalized()); // エッジに垂直な法線
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

	// 軸を取得
	auto axesA = GetAxes(triA);
	auto axesB = GetAxes(triB);

	// Aの軸で分離軸を確認
	for (const auto& axis : axesA) {
		auto rangeA = ProjectTriangle(triA, axis);
		auto rangeB = ProjectTriangle(triB, axis);
		if (!CheckOverlap(rangeA, rangeB)) {
			return false; // 分離軸が存在
		}
	}

	// Bの軸で分離軸を確認
	for (const auto& axis : axesB) {
		auto rangeA = ProjectTriangle(triA, axis);
		auto rangeB = ProjectTriangle(triB, axis);
		if (!CheckOverlap(rangeA, rangeB)) {
			return false; // 分離軸が存在
		}
	}

	return true; // 衝突している
}


/// @brief 凸包を求める関数
/// @param points 入力点集合
/// @return 凸包の頂点リスト
static inline vector<Vector2> ComputeConvexHull(std::vector<Vector2> points) {
	std::sort(points.begin(), points.end()); // x, y の順でソート

	// 下部凸包を構築
	std::vector<Vector2> lower;
	for (const auto& p : points) {
		while (lower.size() >= 2) {
			Vector2 q = lower[lower.size() - 2];
			Vector2 r = lower[lower.size() - 1];
			if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0) { // 時計回りまたは直線
				lower.pop_back();
			}
			else {
				break;
			}
		}
		lower.push_back(p);
	}

	// 上部凸包を構築
	std::vector<Vector2> upper;
	for (auto it = points.rbegin(); it != points.rend(); ++it) {
		const auto& p = *it;
		while (upper.size() >= 2) {
			Vector2 q = upper[upper.size() - 2];
			Vector2 r = upper[upper.size() - 1];
			if ((r.x - q.x) * (p.y - q.y) - (r.y - q.y) * (p.x - q.x) <= 0) { // 時計回りまたは直線
				upper.pop_back();
			}
			else {
				break;
			}
		}
		upper.push_back(p);
	}

	// 上部と下部を統合（最初と最後の点が重複しているので削除）
	lower.pop_back();
	upper.pop_back();
	lower.insert(lower.end(), upper.begin(), upper.end());
	return lower;
}

/// @brief 凸包を2つの三角形に分割する
/// @param hull 凸包の頂点リスト
/// @return 2つの三角形に分割した結果
static inline std::vector<std::vector<Vector2>> SplitIntoTriangles(const std::vector<Vector2>& hull) {
	std::vector<std::vector<Vector2>> triangles;
	if (hull.size() < 4) {
		throw std::runtime_error("凸包に少なくとも4点が必要です。");
	}

	// 基準点を選び、扇形に三角形を作る
	const auto& base = hull[0];
	for (size_t i = 1; i < hull.size() - 1; ++i) {
		triangles.push_back({ base, hull[i], hull[i + 1] });
	}
	return triangles;
}

/// @brief 2つの三角形ポリゴンを内包する四角形を構成する2つの三角形ポリゴンに変換する関数
/// @param triA 一つ目の三角形
/// @param triB 二つ目の三角形
/// @return 四角形を構成する2つの三角形
static inline std::vector<std::vector<Vector2>> ConvertTrianglesToBoundingTriangles(
	const std::vector<Vector2>& triA,
	const std::vector<Vector2>& triB) {

	if (triA.size() != 3 || triB.size() != 3) {
		throw std::invalid_argument("三角形は3点で構成される必要があります。");
	}

	// 頂点をまとめる
	std::vector<Vector2> points = triA;
	points.insert(points.end(), triB.begin(), triB.end());

	// 凸包を計算
	std::vector<Vector2> hull = ComputeConvexHull(points);

	// 凸包を2つの三角形に分割
	return SplitIntoTriangles(hull);
}

static inline bool CollisionBox(Position2D boxApos, Size2D boxASize, Position2D boxBpos, Size2D boxBSize) {

	return !(abs(boxApos.x - boxBpos.x) >= (boxASize.x / 2.0f + boxBSize.x / 2.0f) ||
		abs(boxApos.y - boxBpos.y) >= (boxASize.y / 2.0f + boxBSize.y / 2.0f));
}

// 指定した軸の矩形と矩形のめり込み量を返す
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

// 円と円の衝突判定
static inline bool IsCollisionCircle(Position2D pos1, float radi1, Position2D pos2, float radi2)
{
	Position2D pos = { pos2.x - pos1.x,pos2.y - pos1.y };
	float dis = sqrt(pos.x * pos.x + pos.y * pos.y);
	float rad = radi1 + radi2;

	// 中心距離が二つの円の半径を足したものよりも短ければ当たっている
	if (dis < rad)
	{
		return true;
	}

	return false;
}

/// @brief 矩形と円の衝突判定
/// @param rectCenter  
/// @param rectSize
/// @param circleCenter 
/// @param radius 
/// @return 
static inline bool IsCollisionRect2Circle(Position2D rectCenter, Size2D rectSize, Position2D circleCenter, float radius)
{
	Size2D size = { radius * 2.0f ,radius * 2.0f };
	if (!CollisionBox(rectCenter, rectSize, circleCenter, size)) return false;

	// 領域A衝突判定
	Position2D posA1 = { rectCenter.x - (rectSize.x / 2),rectCenter.y - (rectSize.y / 2) - radius };
	Position2D posA2 = { rectCenter.x + (rectSize.x / 2),rectCenter.y + (rectSize.y / 2) + radius };
	if (posA1.x <= circleCenter.x && circleCenter.x <= posA2.x &&
		posA1.y <= circleCenter.y && circleCenter.y <= posA2.y)
	{
		return true;
	}

	// 領域B衝突判定
	Position2D posB1 = { rectCenter.x - (rectSize.x / 2) - radius,rectCenter.y - (rectSize.y / 2) };
	Position2D posB2 = { rectCenter.x + (rectSize.x / 2) + radius,rectCenter.y + (rectSize.y / 2) };
	if (posB1.x <= circleCenter.x && circleCenter.x <= posB2.x &&
		posB1.y <= circleCenter.y && circleCenter.y <= posB2.y)
	{
		return true;
	}

	// 領域C衝突判定
	Position2D posC = { rectCenter.x - (rectSize.x / 2) ,rectCenter.y - (rectSize.y / 2) };
	if (IsCollisionCircle(posC, radius, circleCenter, radius)) return true;

	// 領域D衝突判定
	Position2D posD = { rectCenter.x + (rectSize.x / 2) ,rectCenter.y - (rectSize.y / 2) };
	if (IsCollisionCircle(posD, radius, circleCenter, radius)) return true;

	// 領域E衝突判定
	Position2D posE = { rectCenter.x - (rectSize.x / 2) ,rectCenter.y + (rectSize.y / 2) };
	if (IsCollisionCircle(posE, radius, circleCenter, radius)) return true;

	// 領域F衝突判定
	Position2D posF = { rectCenter.x + (rectSize.x / 2) ,rectCenter.y + (rectSize.y / 2) };
	if (IsCollisionCircle(posF, radius, circleCenter, radius)) return true;


	return false;
}

static inline void LoadCsvData(string filePath, vector<int>& mapVec, int& width, int& height)
{
	mapVec.clear();
	width = 0;
	height = 0;

	// ファイルのパス
	string path = filePath;

	// ファイルの読み込み
	ifstream ifs = ifstream(path.c_str());
	if (!ifs)
	{
		// エラーが発生
		return;
	}

	// ファイルを1行ずつ読み込んで、lineに格納する
	string line;					// 1行の文字情報
	vector<string> split;		// 1文字1文字の文字列情報
	int chipNo = 0;

	// ファイルを1行ずつ読み込む
	while (getline(ifs, line))
	{
		// 1行分の文字列をカンマ区切りで分割
		split = Split(line, ',');

		width = (int)split.size();

		for (int x = 0; x < split.size(); x++)
		{

			// string型からint型に変換
			int chipNo = stoi(split[x]);

			// 配列にマップチップ番号を格納
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
	scale.x = XMVectorGetX(XMVector3Length(matrix.r[0]));  // X軸のスケール
	scale.y = XMVectorGetY(XMVector3Length(matrix.r[1]));  // Y軸のスケール
	scale.z = XMVectorGetZ(XMVector3Length(matrix.r[2]));  // Z軸のスケール
	return scale;
}

static inline XMMATRIX GetRotationMatrixFromMatrix(const XMMATRIX& matrix)
{
	// 各軸ベクトル（X, Y, Z）を正規化してスケールを取り除く
	XMVECTOR xAxis = XMVector3Normalize(matrix.r[0]); // X軸
	XMVECTOR yAxis = XMVector3Normalize(matrix.r[1]); // Y軸
	XMVECTOR zAxis = XMVector3Normalize(matrix.r[2]); // Z軸

	// 回転成分のみの行列を構築
	return XMMATRIX(xAxis, yAxis, zAxis, XMVectorSet(0, 0, 0, 1));
}

static inline void GetEulerAnglesFromMatrix(const XMMATRIX& mat, float& roll, float& pitch, float& yaw) {
	// 回転行列の要素を取得
	float m00 = mat.r[0].m128_f32[0];
	float m01 = mat.r[0].m128_f32[1];
	float m02 = mat.r[0].m128_f32[2];
	float m10 = mat.r[1].m128_f32[0];
	float m11 = mat.r[1].m128_f32[1];
	float m12 = mat.r[1].m128_f32[2];
	float m20 = mat.r[2].m128_f32[0];
	float m21 = mat.r[2].m128_f32[1];
	float m22 = mat.r[2].m128_f32[2];

	// ピッチ（Y軸回り）を取得
	pitch = std::asin(-m20);

	// ガンマ行列の特異値（ピッチ角が±90度）を考慮
	if (std::abs(m20) < 0.9999f) {
		// ピッチ角が通常範囲内の場合
		yaw = std::atan2(m10, m00);
		roll = std::atan2(m21, m22);
	}
	else {
		// ピッチ角が±90度の特異値の場合
		yaw = std::atan2(-m01, m11);
		roll = 0.0f;
	}
}


/// @brief リソースバリアをセット
/// @param Resource 変更するリソース
/// @param before 変更前
/// @param after 変更後
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
