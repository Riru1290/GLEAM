#include "Quaternion.h"

Quaternion::Quaternion()
{
	w = 1.0;
	x = y = z = 0.0;
}

Quaternion::Quaternion(const Vector3& rad)
{
    Quaternion q = Euler(rad.x, rad.y, rad.z);
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

Quaternion::Quaternion(double ww,double wx, double wy, double wz)
{
    w = ww;
	x = wx;
	y = wy;
	z = wz;
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::Euler(const Vector3& rad)
{
    return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(double radX, double radY, double radZ)
{
    Quaternion ret = Quaternion();

    radX = RadIn2PI(radX);
    radY = RadIn2PI(radY);
    radZ = RadIn2PI(radZ);

    double cosZ = cos(radZ / 2.0f);
    double sinZ = sin(radZ / 2.0f);
    double cosX = cos(radX / 2.0f);
    double sinX = sin(radX / 2.0f);
    double cosY = cos(radY / 2.0f);
    double sinY = sin(radY / 2.0f);

    ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
    ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
    ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    return ret;
}

Quaternion Quaternion::Mult(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion ret = Quaternion();
    double d1, d2, d3, d4;

    // wÇÃåvéZ 
    d1 = q1.w * q2.w;
    d2 = -q1.x * q2.x;
    d3 = -q1.y * q2.y;
    d4 = -q1.z * q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // xÇÃåvéZ 
    d1 = q1.w * q2.x;
    d2 = q2.w * q1.x;
    d3 = q1.y * q2.z;
    d4 = -q1.z * q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // yÇÃåvéZ
    d1 = q1.w * q2.y;
    d2 = q2.w * q1.y;
    d3 = q1.z * q2.x;
    d4 = -q1.x * q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // zÇÃåvéZ
    d1 = q1.w * q2.z;
    d2 = q2.w * q1.z;
    d3 = q1.x * q2.y;
    d4 = -q1.y * q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;
}

Quaternion Quaternion::Mult(const Quaternion& q2) const
{
    Quaternion q1 = Quaternion(w, x, y, z);

    Quaternion ret = Quaternion();
    double d1, d2, d3, d4;

    // wÇÃåvéZ 
    d1 = q1.w * q2.w;
    d2 = -q1.x * q2.x;
    d3 = -q1.y * q2.y;
    d4 = -q1.z * q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // xÇÃåvéZ 
    d1 = q1.w * q2.x;
    d2 = q2.w * q1.x;
    d3 = q1.y * q2.z;
    d4 = -q1.z * q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // yÇÃåvéZ
    d1 = q1.w * q2.y;
    d2 = q2.w * q1.y;
    d3 = q1.z * q2.x;
    d4 = -q1.x * q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // zÇÃåvéZ
    d1 = q1.w * q2.z;
    d2 = q2.w * q1.z;
    d3 = q1.x * q2.y;
    d4 = -q1.y * q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;
}

Quaternion Quaternion::AngleAxis(double rad, Vector3 axis)
{
    Quaternion ret = Quaternion();

    double norm;
    double c, s;

    // UnityÇ…çáÇÌÇπÇÈ
    //ret.w = ret.x = ret.y = ret.z = 0.0;
    ret.w = 1.0;
    ret.x = ret.y = ret.z = 0.0;

    norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
    if (norm <= 0.0f)
    {
        return ret;
    }

    norm = 1.0 / sqrt(norm);
    axis.x = (float)(axis.x * norm);
    axis.y = (float)(axis.y * norm);
    axis.z = (float)(axis.z * norm);

    c = cos(0.5f * rad);
    s = sin(0.5f * rad);

    ret.w = c;
    ret.x = s * axis.x;
    ret.y = s * axis.y;
    ret.z = s * axis.z;

    return ret;
}

Vector3 Quaternion::PosAxis(const Quaternion& q, Vector3 pos)
{
    // à íuèÓïÒÇ…âÒì]èÓïÒÇîΩâfÇ≥ÇπÇÈ
    // pos' = qÅEposÅEq(-1)
    Quaternion tmp = Quaternion();
    tmp = tmp.Mult(q);
    tmp = tmp.Mult(Quaternion(0.0f, pos.x, pos.y, pos.z));
    tmp = tmp.Mult(q.Inverse());
    return { (float)tmp.x, (float)tmp.y, (float)tmp.z };
}

Vector3 Quaternion::PosAxis(Vector3 pos) const
{
    return PosAxis(Quaternion(w, x, y, z), pos);
}

Vector3 Quaternion::ToEuler(const Quaternion& q)
{
    Vector3 ret;

    double r11 = 2 * (q.x * q.z + q.w * q.y);
    double r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    double r21 = -2 * (q.y * q.z - q.w * q.x);
    double r31 = 2 * (q.x * q.y + q.w * q.z);
    double r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;

    ret.x = static_cast<float>(asin(r21));
    ret.y = static_cast<float>(atan2(r11, r12));
    ret.z = static_cast<float>(atan2(r31, r32));

    return ret;
}

Vector3 Quaternion::ToEuler(void) const
{
    return ToEuler(Quaternion(w, x, y, z));
}

XMMATRIX Quaternion::ToMatrix(const Quaternion& q)
{
    XMMATRIX mat;

    XMFLOAT4 fq = { (float)q.x, (float)q.y, (float)q.z, (float)q.w };

    float sx = fq.x * fq.x * 2.0f;
    float sy = fq.y * fq.y * 2.0f;
    float sz = fq.z * fq.z * 2.0f;
    float cx = fq.y * fq.z * 2.0f;
    float cy = fq.x * fq.z * 2.0f;
    float cz = fq.x * fq.y * 2.0f;
    float wx = fq.w * fq.x * 2.0f;
    float wy = fq.w * fq.y * 2.0f;
    float wz = fq.w * fq.z * 2.0f;
    
    mat.r[0].m128_f32[0] = 1.0f - (sy + sz);	mat.r[0].m128_f32[1] = cz + wz;			    mat.r[0].m128_f32[2] = cy - wy;			    mat.r[0].m128_f32[3] = 0.0f;
    mat.r[1].m128_f32[0] = cz - wz;			    mat.r[1].m128_f32[1] = 1.0f - (sx + sz);	mat.r[1].m128_f32[2] = cx + wx;			    mat.r[1].m128_f32[3] = 0.0f;
    mat.r[2].m128_f32[0] = cy + wy;			    mat.r[2].m128_f32[1] = cx - wx;			    mat.r[2].m128_f32[2] = 1.0f - (sx + sy);	mat.r[2].m128_f32[3] = 0.0f;
    mat.r[3].m128_f32[0] = 0.0f;				mat.r[3].m128_f32[1] = 0.0f;			    mat.r[3].m128_f32[2] = 0.0f;		        mat.r[3].m128_f32[3] = 1.0f;

    return mat;
}

XMMATRIX Quaternion::ToMatrix(void) const
{
    return ToMatrix(Quaternion(w, x, y, z));
}

Vector3 Quaternion::GetForward(void) const
{
    return GetDir(Position3D(0.0f,0.0f,1.0f));
}

Vector3 Quaternion::GetBack(void) const
{
    return GetDir(Position3D(0.0f, 0.0f, -1.0f));
}

Vector3 Quaternion::GetRight(void) const
{
    return GetDir(Position3D(1.0f, 0.0f, 0.0f));
}

Vector3 Quaternion::GetLeft(void) const
{
    return GetDir(Position3D(-1.0f, 0.0f, 0.0f));
}

Vector3 Quaternion::GetUp(void) const
{
    return GetDir(Position3D(0.0f, 1.0f, 0.0f));
}

Vector3 Quaternion::GetDown(void) const
{
    return GetDir(Position3D(0.0f, -1.0f, 0.0f));
}

Quaternion Quaternion::LookRotation(Vector3 dir)
{
    Vector3 up = { 0.0f, 1.0f, 0.0f };
    return LookRotation(dir, up);
}

Quaternion Quaternion::LookRotation(Vector3 dir, Vector3 up)
{
    dir.Normalize();
    Vector3 right = Cross(up, dir).Normalized();
    up = Cross(dir, right);
    auto m00 = right.x;
    auto m01 = right.y;
    auto m02 = right.z;
    auto m10 = up.x;
    auto m11 = up.y;
    auto m12 = up.z;
    auto m20 = dir.x;
    auto m21 = dir.y;
    auto m22 = dir.z;


    float num8 = (m00 + m11) + m22;
    auto quaternion = Quaternion();
    if (num8 > 0.0f)
    {
        double num = sqrt(num8 + 1.0);
        quaternion.w = num * 0.5;
        num = 0.5 / num;
        quaternion.x = ((double)m12 - m21) * num;
        quaternion.y = ((double)m20 - m02) * num;
        quaternion.z = ((double)m01 - m10) * num;
        return quaternion.Normalized();
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        //// xÇ∆wÇ™ãtÅH
        //auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        //auto num4 = 0.5f / num7;
        //quaternion.x = 0.5f * num7;
        //quaternion.y = (m01 + m10) * num4;
        //quaternion.z = (m02 + m20) * num4;
        //quaternion.w = (m12 - m21) * num4;
        //return quaternion.Normalized();
        auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        auto num4 = 0.5f / num7;
        quaternion.x = ((double)m12 - m21) * num4;
        quaternion.y = ((double)m01 + m10) * num4;
        quaternion.z = ((double)m02 + m20) * num4;
        quaternion.w = 0.5 * num7;
        return quaternion.Normalized();
    }
    if (m11 > m22)
    {
        auto num6 = sqrt(((1.0f + m11) - m00) - m22);
        auto num3 = 0.5f / num6;
        quaternion.x = ((double)m10 + m01) * num3;
        quaternion.y = 0.5 * num6;
        quaternion.z = ((double)m21 + m12) * num3;
        quaternion.w = ((double)m20 - m02) * num3;
        return quaternion.Normalized();
    }
    auto num5 = sqrt(((1.0f + m22) - m00) - m11);
    auto num2 = 0.5f / num5;
    quaternion.x = ((double)m20 + m02) * num2;
    quaternion.y = ((double)m21 + m12) * num2;
    quaternion.z = 0.5 * num5;
    quaternion.w = ((double)m01 - m10) * num2;
    return quaternion.Normalized();

}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
    float scale = 1.0f / static_cast<float>(q.Length());
    Vector3 v = Vector3((float)q.x, (float)q.y, (float)q.z) * scale;
    Quaternion ret = Quaternion(q.w * scale, v.x, v.y, v.z);
    return ret;
}

Quaternion Quaternion::Normalized(void) const
{
    double mag = sqrt(w * w + x * x + y * y + z * z);
    return Quaternion(w / mag, x / mag, y / mag, z / mag);
}

void Quaternion::Normalize(void)
{
    double mag = sqrt(w * w + x * x + y * y + z * z);

    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;
}

Quaternion Quaternion::Inverse(void) const
{
    double n = 1.0f / (w * w + x * x + y * y + z * z);
    Quaternion tmp = Quaternion(w, -x, -y, -z);
    return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);
}

double Quaternion::Length(void) const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

double Quaternion::LengthSquared(void) const
{
    return x * x + y * y + z * z + w * w;
}

Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, double t)
{
    if (t > 1) t = 1;
    if (t < 0) t = 0;
    return SlerpUnclamped(from, to, (float)t);

}

Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{

    // if either input is zero, return the other.
    if (a.LengthSquared() == 0.0f)
    {
        if (b.LengthSquared() == 0.0f)
        {
            return Identity();
        }
        return b;
    }
    else if (b.LengthSquared() == 0.0f)
    {
        return a;
    }


    float cosHalfAngle = (float)(a.w * b.w) + Dot(a.xyz(), b.xyz());

    if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
    {
        // angle = 0.0f, so just return one input.
        return a;
    }
    else if (cosHalfAngle < 0.0f)
    {
        //b.xyz() = -b.xyz();
        b.x = b.x * -1.0f;
        b.y = b.y * -1.0f;
        b.z = b.z * -1.0f;
        b.w = -b.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if (cosHalfAngle < 0.99f)
    {
        // do proper slerp for big angles
        float halfAngle = acosf(cosHalfAngle);
        float sinHalfAngle = sinf(halfAngle);
        float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
        blendA = sinf(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
        blendB = sinf(halfAngle * t) * oneOverSinHalfAngle;
    }
    else
    {
        // do lerp if angle is really small.
        blendA = 1.0f - t;
        blendB = t;
    }

    //Quaternion result = Quaternion(blendA * a.xyz() + blendB * b.xyz(), blendA * a.w + blendB * b.w);
    Vector3 v = (a.xyz() * blendA) + (b.xyz() * blendB);
    //Quaternion result = Quaternion(v.x, v.y, v.z, blendA * a.w + blendB * b.w);
    Quaternion result = Quaternion(blendA * a.w + blendB * b.w, v.x, v.y, v.z);
    if (result.LengthSquared() > 0.0f)
    {
        return Normalize(result);
    }
    else
    {
        return Identity();
    }

}

Quaternion Quaternion::Identity(void)
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Vector3 Quaternion::xyz() const
{
    return { (float)x, (float)y, (float)z };
}

Vector3 Quaternion::GetDir(Vector3 dir) const
{
    return PosAxis(dir);
}
