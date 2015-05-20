#pragma once

#include "ZFXPrerequisites.h"
#include "ZFXVector3.h"
#include "ZFXMatrix3.h"
#include "ZFXMath.h"

namespace ZFX
{
	class ZFXExport Quaternion
	{
	public:
		float32 x, y, z, w;

	public:
		inline Quaternion()
			: x(0), y(0), z(0), w(1)
		{
		}

		inline Quaternion(float32 x, float32 y, float32 z, float32 w)
			: x(x), y(y), z(z), w(w)
		{
		}

		inline Quaternion(const Matrix3& rot)
		{
			FromRotationMatrix(rot);
		}

		inline Quaternion(const float32 angle, const Vector3& axis)
		{
			FromAngleAxis(angle, axis);
		}

		inline void Swap(Quaternion& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
			std::swap(w, other.w);
		}

		inline bool operator==(const Quaternion& q) const
		{
			return (q.x == x) && (q.y == y) && (q.z == z)
				&& (q.w == w);
		}

		inline bool operator!=(const Quaternion& q) const
		{
			return !operator==(q);
		}

		inline bool IsNaN() const
		{
			return x != x || y != y || z != z || w != w;
		}

		inline Quaternion& operator=(const Quaternion& q)
		{
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
			return *this;
		}

		Quaternion operator+(const Quaternion& q) const;

		Quaternion operator-(const Quaternion& q) const;

		Quaternion operator*(const Quaternion& q) const;

		Quaternion operator*(float32 scalar) const;

		Vector3 operator*(const Vector3& v) const;

		ZFXExport friend Quaternion operator*(float32 scalar, const Quaternion& q);

		Quaternion operator-() const;

		void FromRotationMatrix(const Matrix3& rot);

		void ToRotationMatrix(Matrix3& rot) const;

		void FromAngleAxis(const float32 angle, const Vector3& axis);

		void ToAngleAxis(float32& angle, Vector3& axis) const;

		Vector3 GetXAxis() const;

		Vector3 GetYAxis() const;

		Vector3 GetZAxis() const;

		float32 DotProduct(const Quaternion& q) const;

		float32 Norm() const;

		float32 Normalise();

		Quaternion Inverse() const;

		Quaternion UnitInverse() const;

	public:
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;
	};
}