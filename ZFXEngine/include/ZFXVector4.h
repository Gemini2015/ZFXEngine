#ifndef _ZFX_VECTOR_4_H_
#define _ZFX_VECTOR_4_H_

#include "PILPrerequisites.h"
#include "ZFXPrerequisites.h"

#include <assert.h>

namespace ZFX
{
	class Vector4
	{
	public:
		union
		{
			struct
			{
				float32 x, y, z, w;
			};
			float32 v[4];
		};

	public:
		inline Vector4()
		{
		}

		inline Vector4(float32 x, float32 y, float32 z, float32 w)
			: x(x), y(y), z(z), w(w)
		{
		}

		inline Vector4(float32 x, float32 y, float32 z)
			: x(x), y(y), z(z), w(1.0f)
		{
		}

		inline Vector4(float32 v[4])
			: x(v[0]), y(v[1]), z(v[2]), w(v[3])
		{
		}

		inline Vector4(int32 v[4])
		{
			x = (float32)v[0];
			x = (float32)v[0];
			x = (float32)v[0];
			x = (float32)v[0];
		}

		inline Vector4(float32 scalar)
			: x(scalar), y(scalar), z(scalar), w(scalar)
		{
		}

		float32 GetLength();

		float32 GetSqrLength();

		Vector4 Normalize();

		Vector4 VectorTo(const Vector4& v);

		inline void Swap(Vector4& v)
		{
			std::swap(x, v.x);
			std::swap(y, v.y);
			std::swap(z, v.z);
			std::swap(w, v.w);
		}

		inline float32 operator[](const size_t i) const
		{
			assert(i < 4);
			return v[i];
		}

		inline float32& operator[](const size_t i)
		{
			assert(i < 4);
			return v[i];
		}

		inline Vector4& operator=(const Vector4& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		inline bool operator==(const Vector4& v) const
		{
			return (x == v.x && y == v.y
				&& z == v.z && w == v.w);
		}

		inline bool operator!=(const Vector4& v) const
		{
			return (x != v.x || y != v.y
				|| z != v.z || w == v.w);
		}

		inline Vector4 operator + (const Vector4& v) const
		{
			return Vector4(
				x + v.x,
				y + v.y,
				z + v.z,
				w + v.w);
		}

		inline Vector4 operator - (const Vector4& v) const
		{
			return Vector4(
				x - v.x,
				y - v.y,
				z - v.z,
				w - v.w);
		}

		inline Vector4 operator * (const float32 scalar) const
		{
			return Vector4(
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar);
		}

		inline Vector4 operator * (const Vector4& v) const
		{
			return Vector4(
				v.x * x,
				v.y * y,
				v.z * z,
				v.w * w);
		}

		inline Vector4 operator / (const float32 scalar) const
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			return Vector4(
				x * fInv,
				y * fInv,
				z * fInv,
				w * fInv);
		}

		inline Vector4 operator / (const Vector4& v) const
		{
			return Vector4(
				x / v.x,
				y / v.y,
				z / v.z,
				w / v.w);
		}

		inline const Vector4& operator + () const
		{
			return *this;
		}

		inline Vector4 operator - () const
		{
			return Vector4(-x, -y, -z, -w);
		}

		inline friend Vector4 operator * (const float32 scalar, const Vector4& v)
		{
			return Vector4(
				scalar * v.x,
				scalar * v.y,
				scalar * v.z,
				scalar * v.w);
		}

		inline friend Vector4 operator / (const float32 scalar, const Vector4& v)
		{
			return Vector4(
				scalar / v.x,
				scalar / v.y,
				scalar / v.z,
				scalar / v.w);
		}

		inline friend Vector4 operator + (const Vector4& va, const float32 vb)
		{
			return Vector4(
				va.x + vb,
				va.y + vb,
				va.z + vb,
				va.w + vb);
		}

		inline friend Vector4 operator + (const float32 va, const Vector4& vb)
		{
			return Vector4(
				va + vb.x,
				va + vb.y,
				va + vb.z,
				va + vb.w);
		}

		inline friend Vector4 operator - (const Vector4& va, float32 vb)
		{
			return Vector4(
				va.x - vb,
				va.y - vb,
				va.z - vb,
				va.w - vb);
		}

		inline friend Vector4 operator - (const float32 va, const Vector4& vb)
		{
			return Vector4(
				va - vb.x,
				va - vb.y,
				va - vb.z,
				va - vb.w);
		}

		// arithmetic updates
		inline Vector4& operator += (const Vector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;

			return *this;
		}

		inline Vector4& operator -= (const Vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}

		inline Vector4& operator *= (const float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		inline Vector4& operator += (const float32 scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			w += scalar;
			return *this;
		}

		inline Vector4& operator -= (const float32 scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			w -= scalar;
			return *this;
		}

		inline Vector4& operator *= (const Vector4& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;

			return *this;
		}

		inline Vector4& operator /= (const float32 scalar)
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;
			w *= fInv;

			return *this;
		}

		inline Vector4& operator /= (const Vector4& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;

			return *this;
		}

		inline float32 DotProduct(const Vector4& v) const
		{
			return x * v.x + y * v.y + z * v.z + w * v.w;
		}

		static const Vector4 ZERO;
		static const Vector4 ONE;

	};
}

#endif