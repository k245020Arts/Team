// Quaternion.h
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

    Quaternion operator*(const Quaternion& b) const {
        // Hamilton product
        return Quaternion(
            w * b.w - x * b.x - y * b.y - z * b.z,
            w * b.x + x * b.w + y * b.z - z * b.y,
            w * b.y - x * b.z + y * b.w + z * b.x,
            w * b.z + x * b.y - y * b.x + z * b.w
        );
    }

    Quaternion Normalize() const {
        const float n = sqrtf(w*w + x*x + y*y + z*z);
        if (n <= 0.0f) return Quaternion(1,0,0,0);
        return Quaternion(w/n, x/n, y/n, z/n);
    }

    // ベクトルをクォータニオンで回転（v' = q * v * q^-1）
    VECTOR3 Rotate(const VECTOR3& v) const {
        // 最適化版: t = 2 * cross(q.xyz, v); v' = v + q.w * t + cross(q.xyz, t)
        VECTOR3 qv{ x, y, z };
        VECTOR3 t = VCross(qv, v) * 2.0f;
        VECTOR3 vp = v + t * w + VCross(qv, t);
        return vp;
    }

    // クォータニオン -> オイラー (X=pitch, Y=yaw, Z=roll)
    // 注意: 抽出の回転順は使用側と同じにすること（ここでは Y-X-Z に合わせた近似抽出）
    VECTOR3 ToEuler() const {
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