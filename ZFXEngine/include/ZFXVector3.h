#ifndef _ZFX_VECTOR_3_H_
#define _ZFX_VECTOR_3_H_

#include "ZFXPrerequisites.h"

namespace ZFX
{
	class ZFXExport Vector3
	{
	public:
		union
		{
			struct
			{
				float32 x, y, z;
			};
			float32 val[3];
		};

	public:
		inline Vector3()
		{
		}

		inline Vector3(float32 x, float32 y, float32 z)
			: x(x), y(y), z(z)
		{
		}

		inline Vector3(float32 v[3])
			: x(v[0]), y(v[1]), z(v[2])
		{
		}

		inline Vector3(int32 v[3])
		{
			x = (float32)v[0];
			x = (float32)v[0];
			x = (float32)v[0];
		}

		inline Vector3(float32 scalar)
			: x(scalar), y(scalar), z(scalar)
		{
		}

		float32 GetLength() const;

		float32 GetSqrLength() const;

		Vector3 Normalize();

		Vector3 VectorTo(const Vector3& v);

		float32 GetDistance(const Vector3& v) const;

		float32 GetSqrDistance(const Vector3& v) const;

		Vector3 CrossProduct(const Vector3& v) const;

		Vector3 Reflect(const Vector3& normal) const;

		inline Vector3 MidPoint(const Vector3& v) const
		{
			return Vector3(
				(x + v.x) * 0.5f,
				(y + v.y) * 0.5f,
				(z + v.z) * 0.5f);
		}

		inline void Swap(Vector3& v)
		{
			std::swap(x, v.x);
			std::swap(y, v.y);
			std::swap(z, v.z);
		}

		inline float32 operator[](const size_t i) const
		{
			assert(i < 3);
			return val[i];
		}

		inline float32& operator[](const size_t i)
		{
			assert(i < 3);
			return val[i];
		}

		inline Vector3& operator=(const Vector3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		inline bool operator==(const Vector3& v) const
		{
			return (x == v.x && y == v.y
				&& z == v.z);
		}

		inline bool operator!=(const Vector3& v) const
		{
			return (x != v.x || y != v.y
				|| z != v.z);
		}

		inline Vector3 operator + (const Vector3& v) const
		{
			return Vector3(
				x + v.x,
				y + v.y,
				z + v.z);
		}

		inline Vector3 operator - (const Vector3& v) const
		{
			return Vector3(
				x - v.x,
				y - v.y,
				z - v.z);
		}

		inline Vector3 operator * (const float32 scalar) const
		{
			return Vector3(
				x * scalar,
				y * scalar,
				z * scalar);
		}

		inline Vector3 operator * (const Vector3& v) const
		{
			return Vector3(
				v.x * x,
				v.y * y,
				v.z * z);
		}

		inline Vector3 operator / (const float32 scalar) const
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			return Vector3(
				x * fInv,
				y * fInv,
				z * fInv);
		}

		inline Vector3 operator / (const Vector3& v) const
		{
			return Vector3(
				x / v.x,
				y / v.y,
				z / v.z);
		}

		inline const Vector3& operator + () const
		{
			return *this;
		}

		inline Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}

		inline friend Vector3 operator * (const float32 scalar, const Vector3& v)
		{
			return Vector3(
				scalar * v.x,
				scalar * v.y,
				scalar * v.z);
		}

		inline friend Vector3 operator / (const float32 scalar, const Vector3& v)
		{
			return Vector3(
				scalar / v.x,
				scalar / v.y,
				scalar / v.z);
		}

		inline friend Vector3 operator + (const Vector3& va, const float32 vb)
		{
			return Vector3(
				va.x + vb,
				va.y + vb,
				va.z + vb);
		}

		inline friend Vector3 operator + (const float32 va, const Vector3& vb)
		{
			return Vector3(
				va + vb.x,
				va + vb.y,
				va + vb.z);
		}

		inline friend Vector3 operator - (const Vector3& va, float32 vb)
		{
			return Vector3(
				va.x - vb,
				va.y - vb,
				va.z - vb);
		}

		inline friend Vector3 operator - (const float32 va, const Vector3& vb)
		{
			return Vector3(
				va - vb.x,
				va - vb.y,
				va - vb.z);
		}

		// arithmetic updates
		inline Vector3& operator += (const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		inline Vector3& operator -= (const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		inline Vector3& operator *= (const float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		inline Vector3& operator += (const float32 scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		inline Vector3& operator -= (const float32 scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			return *this;
		}

		inline Vector3& operator *= (const Vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;

			return *this;
		}

		inline Vector3& operator /= (const float32 scalar)
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;

			return *this;
		}

		inline Vector3& operator /= (const Vector3& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;

			return *this;
		}

		inline float32 DotProduct(const Vector3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		static const Vector3 ORIGIN;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 ONE;
	};
}

#endif