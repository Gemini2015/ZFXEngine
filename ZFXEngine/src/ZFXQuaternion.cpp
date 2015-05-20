#include "ZFXQuaternion.h"


namespace ZFX
{
	const Quaternion Quaternion::ZERO(0, 0, 0, 0);

	const Quaternion Quaternion::IDENTITY(0, 0, 0, 1);

	ZFX::Quaternion Quaternion::operator+(const Quaternion& q) const
	{
		return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	ZFX::Quaternion Quaternion::operator-(const Quaternion& q) const
	{
		return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	ZFX::Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		throw std::logic_error("todo");
	}

	ZFX::Quaternion Quaternion::operator*(float32 scalar) const
	{
		return Quaternion(scalar * x, scalar * y, scalar * z, scalar * z);
	}

	Quaternion operator*(float32 scalar, const Quaternion& q)
	{
		return Quaternion(scalar * q.x, scalar * q.y, scalar * q.z, scalar * q.z);
	}

	ZFX::Vector3 Quaternion::operator*(const Vector3& v) const
	{
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.CrossProduct(v);
		uuv = qvec.CrossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;
	}

	ZFX::Quaternion Quaternion::operator-() const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	void Quaternion::FromRotationMatrix(const Matrix3& rot)
	{
		float32 trace = rot[0][0] + rot[1][1] + rot[2][2];
		float32 root = 0;
		if (trace > 0.0f)
		{
			root = sqrt(trace + 1.0f);
			w = 0.5f * root;
			root = 0.5f / root;
			x = (rot[2][1] - rot[1][2]) * root;
			y = (rot[0][2] - rot[2][0]) * root;
			z = (rot[1][0] - rot[0][1]) * root;
		}
		else
		{
			static size_t next[3] = { 1, 2, 0 };
			size_t i = 0;
			if (rot[1][1] > rot[0][0])
				i = 1;
			if (rot[2][2] > rot[1][1])
				i = 2;
			size_t j = next[i];
			size_t k = next[j];

			root = sqrt(rot[i][i] - rot[j][j] - rot[k][k] + 1.0f);
			float32* quat[3] = { &x, &y, &z };
			*quat[i] = 0.5f * root;
			root = 0.5f / root;
			w = (rot[k][j] - rot[j][k]) * root;
			*quat[j] = (rot[j][i] + rot[i][j]) * root;
			*quat[k] = (rot[k][i] + rot[i][k]) * root;
		}
	}

	void Quaternion::ToRotationMatrix(Matrix3& rot) const
	{
		float32 tx = x + x;
		float32 ty = y + y;
		float32 tz = z + z;
		float32 twx = tx * w;
		float32 twy = ty * w;
		float32 twz = tz * w;
		float32 txx = tx * x;
		float32 txy = ty * x;
		float32 txz = tz * x;
		float32 tyy = ty * y;
		float32 tyz = tz * y;
		float32 tzz = tz * z;

		rot[0][0] = 1.0f - (tyy + tzz);
		rot[0][1] = txy - twz;
		rot[0][2] = txz + twy;
		rot[1][0] = txy + twz;
		rot[1][1] = 1.0f - (txx + tzz);
		rot[1][2] = tyz - twx;
		rot[2][0] = txz - twy;
		rot[2][1] = tyz + twx;
		rot[2][2] = 1.0f - (txx + tyy);
	}

	void Quaternion::FromAngleAxis(const float32 angle, const Vector3& axis)
	{
		float32 half = 0.5f * angle * Math::Deg2Rad;
		float32 sinv = sin(half);
		w = cos(half);
		x = sinv * axis.x;
		y = sinv * axis.y;
		z = sinv * axis.z;
	}

	void Quaternion::ToAngleAxis(float32& angle, Vector3& axis) const
	{
		float32 length = x*x + y*y + z*z;
		if (length > 0.0f)
		{
			angle = 2.0f * acos(w);
			float32 invlength = 1 / length;
			invlength = sqrt(invlength);
			axis.x = x * invlength;
			axis.y = y * invlength;
			axis.z = z * invlength;
		}
		else
		{
			angle = 0.0f;
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
		}
	}

	ZFX::Vector3 Quaternion::GetXAxis() const
	{
		float32 ty = y + y;
		float32 tz = z + z;
		float32 twy = ty * w;
		float32 twz = tz * w;
		float32 txy = ty * x;
		float32 txz = tz * x;
		float32 tyy = ty * y;
		float32 tzz = tz * z;

		return Vector3(1.0f - (tyy + tzz), txy + twz, txz - twy);
	}

	ZFX::Vector3 Quaternion::GetYAxis() const
	{
		float32 tx = x + x;
		float32 ty = y + y;
		float32 tz = z + z;
		float32 twx = tx * w;
		float32 twz = tz * w;
		float32 txx = tx * x;
		float32 txy = ty * x;
		float32 tyz = tz * y;
		float32 tzz = tz * z;

		return Vector3(txy - twz, 1.0f - (txx + tzz), tyz + twx);
	}

	ZFX::Vector3 Quaternion::GetZAxis() const
	{
		float32 tx = x + x;
		float32 ty = y + y;
		float32 tz = z + z;
		float32 twx = tx * w;
		float32 twy = ty * w;
		float32 txx = tx * x;
		float32 txz = tz * x;
		float32 tyy = ty * y;
		float32 tyz = tz * y;

		return Vector3(txz + twy, tyz - twx, 1.0f - (txx + tyy));
	}

	float32 Quaternion::DotProduct(const Quaternion& q) const
	{
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}

	float32 Quaternion::Norm() const
	{
		return x * x + y * y + z * z + w * w;
	}

	float32 Quaternion::Normalise()
	{
		float32 norm = x * x + y * y + z * z + w * w;
		float32 factor = 1.0f / sqrt(norm);
		*this = *this * factor;
		return norm;
	}

	ZFX::Quaternion Quaternion::Inverse() const
	{
		float32 norm = x * x + y * y + z * z + w * w;
		if (norm > 0.0f)
		{
			float32 invNorm = 1.0f / norm;
			return Quaternion(-x * invNorm, -y * invNorm, -z * invNorm, w * invNorm);
		}
		else
		{
			return ZERO;
		}
	}

	ZFX::Quaternion Quaternion::UnitInverse() const
	{
		return Quaternion(-x, -y, -z, w);
	}

}