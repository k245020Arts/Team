#pragma once
#include "../Transform/Transform.h" // VECTOR3 の定義を利用

struct Quaternion {
    float w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

    static Quaternion FromEuler(const VECTOR3& e) {
        // e.x = pitch (X), e.y = yaw (Y), e.z = roll (Z)
        // 半角のラジアンで与えられている前提
        const float hx = e.x * 0.5f;
        const float hy = e.y * 0.5f;
        const float hz = e.z * 0.5f;

        const float sx = sinf(hx), cx = cosf(hx);
        const float sy = sinf(hy), cy = cosf(hy);
        const float sz = sinf(hz), cz = cosf(hz);

        // 回転行列の組合せ rot = Rz * Rx * Ry に合わせてクォータニオン合成
        // 各軸単位クォータニオン
        Quaternion qx{cx, sx, 0, 0}; // note: w,x,y,z stored as w,x,y,z but here x component in second slot
        Quaternion qy{cy, 0, sy, 0};
        Quaternion qz{cz, 0, 0, sz};

        // q = qz * qx * qy  (回転順に合わせた乗算)
        Quaternion q = qz * qx * qy;
        return q.Normalize();
    }

    Quaternion operator*(const Quaternion& b) {
        // Hamilton product
        return Quaternion(
            w * b.w - x * b.x - y * b.y - z * b.z,
            w * b.x + x * b.w + y * b.z - z * b.y,
            w * b.y - x * b.z + y * b.w + z * b.x,
            w * b.z + x * b.y - y * b.x + z * b.w
        );
    }

    Quaternion Normalize() {
        const float n = sqrtf(w*w + x*x + y*y + z*z);
        if (n <= 0.0f) return Quaternion(1,0,0,0);
        return Quaternion(w/n, x/n, y/n, z/n);
    }

    // ベクトルをクォータニオンで回転（v' = q * v * q^-1）
    VECTOR3 Rotate(const VECTOR3& v)  {
        // 最適化版: t = 2 * cross(q.xyz, v); v' = v + q.w * t + cross(q.xyz, t)
        VECTOR3 qv{ x, y, z };
        VECTOR3 t = VCross(qv, v) * 2.0f;
        VECTOR3 vp = v + t * w + VCross(qv, t);
        return vp;
    }

    // クォータニオン -> オイラー (X=pitch, Y=yaw, Z=roll)
    // 注意: 抽出の回転順は使用側と同じにすること（ここでは Y-X-Z に合わせた近似抽出）
    VECTOR3 ToEuler() {
        // convert to rotation matrix then extract angles
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        // rotation matrix elements
        float m00 = 1.0f - 2.0f * (yy + zz);
        float m01 = 2.0f * (xy - wz);
        float m02 = 2.0f * (xz + wy);

        float m10 = 2.0f * (xy + wz);
        float m11 = 1.0f - 2.0f * (xx + zz);
        float m12 = 2.0f * (yz - wx);

        float m20 = 2.0f * (xz - wy);
        float m21 = 2.0f * (yz + wx);
        float m22 = 1.0f - 2.0f * (xx + yy);

        // ここでは Y (yaw) - X (pitch) - Z (roll) の順序を意識して近似抽出する
        VECTOR3 e;
        // pitch (x)
        if (m21 <= -1.0f) e.x = -DX_PI_F / 2.0f;
        else if (m21 >= 1.0f) e.x = DX_PI_F / 2.0f;
        else e.x = asinf(m21);

        // yaw (y)
        e.y = atan2f(-m20, m22);

        // roll (z)
        e.z = atan2f(-m01, m11);

        return e;
    }
};

// ==============================
// 行列 → クォータニオン
// ==============================
inline Quaternion MToQ(const MATRIX& m)
{
    float t = m.m[0][0] + m.m[1][1] + m.m[2][2];
    Quaternion q;

    if (t > 0.0f) {
        float s = std::sqrt(t + 1.0f) * 2.0f;
        q.w = 0.25f * s;
        q.x = (m.m[2][1] - m.m[1][2]) / s;
        q.y = (m.m[0][2] - m.m[2][0]) / s;
        q.z = (m.m[1][0] - m.m[0][1]) / s;
    }
    else if ((m.m[0][0] > m.m[1][1]) && (m.m[0][0] > m.m[2][2])) {
        float s = std::sqrt(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f;
        q.w = (m.m[2][1] - m.m[1][2]) / s;
        q.x = 0.25f * s;
        q.y = (m.m[0][1] + m.m[1][0]) / s;
        q.z = (m.m[0][2] + m.m[2][0]) / s;
    }
    else if (m.m[1][1] > m.m[2][2]) {
        float s = std::sqrt(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f;
        q.w = (m.m[0][2] - m.m[2][0]) / s;
        q.x = (m.m[0][1] + m.m[1][0]) / s;
        q.y = 0.25f * s;
        q.z = (m.m[1][2] + m.m[2][1]) / s;
    }
    else {
        float s = std::sqrt(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f;
        q.w = (m.m[1][0] - m.m[0][1]) / s;
        q.x = (m.m[0][2] + m.m[2][0]) / s;
        q.y = (m.m[1][2] + m.m[2][1]) / s;
        q.z = 0.25f * s;
    }
    return q;
}


// ==============================
// クォータニオン → 行列
// ==============================
inline MATRIX QToM(const Quaternion& q)
{
    MATRIX m;

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    m.m[0][0] = 1.0f - 2.0f * (yy + zz);
    m.m[0][1] = 2.0f * (xy - wz);
    m.m[0][2] = 2.0f * (xz + wy);
    m.m[0][3] = 0.0f;

    m.m[1][0] = 2.0f * (xy + wz);
    m.m[1][1] = 1.0f - 2.0f * (xx + zz);
    m.m[1][2] = 2.0f * (yz - wx);
    m.m[1][3] = 0.0f;

    m.m[2][0] = 2.0f * (xz - wy);
    m.m[2][1] = 2.0f * (yz + wx);
    m.m[2][2] = 1.0f - 2.0f * (xx + yy);
    m.m[2][3] = 0.0f;

    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;

    return m;
}


// ==============================
// クォータニオン補間（SLERP）
// ==============================
inline Quaternion QSlerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    // 内積
    float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    Quaternion q2b = q2;

    // 逆方向なら反転
    if (dot < 0.0f) {
        dot = -dot;
        q2b.w = -q2.w;
        q2b.x = -q2.x;
        q2b.y = -q2.y;
        q2b.z = -q2.z;
    }

    const float DOT_THRESHOLD = 0.9995f;

    // 角度が小さい → LERP
    if (dot > DOT_THRESHOLD) {
        Quaternion result(
            q1.w + t * (q2b.w - q1.w),
            q1.x + t * (q2b.x - q1.x),
            q1.y + t * (q2b.y - q1.y),
            q1.z + t * (q2b.z - q1.z)
        );
        // 正規化
        float invLen = 1.0f / std::sqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
        result.w *= invLen;
        result.x *= invLen;
        result.y *= invLen;
        result.z *= invLen;
        return result;
    }

    // SLERP
    float theta_0 = std::acos(dot);
    float theta = theta_0 * t;

    float sin_theta = std::sin(theta);
    float sin_theta_0 = std::sin(theta_0);

    float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return Quaternion(
        s0 * q1.w + s1 * q2b.w,
        s0 * q1.x + s1 * q2b.x,
        s0 * q1.y + s1 * q2b.y,
        s0 * q1.z + s1 * q2b.z
    );
}


// ==============================
// ベクトル回転（QApply）
// ==============================
// v' = q * v * q^-1
inline VECTOR QApply(const Quaternion& q, const VECTOR& v)
{
    // q の共役
    Quaternion qc(q.w, -q.x, -q.y, -q.z);

    // v をクォータニオン化
    Quaternion vq(0.0f, v.x, v.y, v.z);

    // q * v
    Quaternion t(
        q.w * vq.w - q.x * vq.x - q.y * vq.y - q.z * vq.z,
        q.w * vq.x + q.x * vq.w + q.y * vq.z - q.z * vq.y,
        q.w * vq.y - q.x * vq.z + q.y * vq.w + q.z * vq.x,
        q.w * vq.z + q.x * vq.y - q.y * vq.x + q.z * vq.w
    );

    // (q * v) * q^-1
    Quaternion r(
        t.w * qc.w - t.x * qc.x - t.y * qc.y - t.z * qc.z,
        t.w * qc.x + t.x * qc.w + t.y * qc.z - t.z * qc.y,
        t.w * qc.y - t.x * qc.z + t.y * qc.w + t.z * qc.x,
        t.w * qc.z + t.x * qc.y - t.y * qc.x + t.z * qc.w
    );

    return VGet(r.x, r.y, r.z);
}