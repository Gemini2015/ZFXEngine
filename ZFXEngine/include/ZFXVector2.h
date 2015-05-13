#pragma once

#include "ZFXPrerequisites.h"

namespace ZFX
{
	class ZFXExport Vector2
	{
	public:
		union
		{
			struct
			{
				float32 x, y;
			};
			struct
			{
				float32 width, height;
			};
			float32 m[2];
		};

	public:
		inline Vector2()
			: x(0.0), y(0.0)
		{
		}

		inline Vector2(const float32 x, const float32 y)
			: x(x), y(y)
		{
		}

		inline explicit Vector2(const float32 v[2])
			: x(v[0]), y(v[1])
		{
		}

		inline explicit Vector2(const int32 v[2])
		{
			x = (float32)v[0];
			y = (float32)v[1];
		}

		inline void Swap(Vector2& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
		}

		inline float32 operator[](const size_t i) const
		{
			assert(i < 2);
			return m[i];
		}

		inline float32& operator[](const size_t i)
		{
			assert(i < 2);
			return m[i];
		}

		inline Vector2& operator=(const Vector2& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		inline Vector2& operator=(const float32 val)
		{
			x = val;
			y = val;
			return *this;
		}

		inline bool operator == (const Vector2& v) const
		{
			return (x == v.x && y == v.y);
		}

		inline bool operator != (const Vector2& v) const
		{
			return (x != v.x || y != v.y);
		}

		inline Vector2 operator + (const Vector2& v) const
		{
			return Vector2(
				x + v.x,
				y + v.y);
		}

		inline Vector2 operator - (const Vector2& v) const
		{
			return Vector2(
				x - v.x,
				y - v.y);
		}

		inline Vector2 operator * (const float32 scalar) const
		{
			return Vector2(
				x * scalar,
				y * scalar);
		}

		inline Vector2 operator * (const Vector2& v) const
		{
			return Vector2(
				x * v.x,
				y * v.y);
		}

		inline Vector2 operator / (const float32 scalar) const
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			return Vector2(
				x * fInv,
				y * fInv);
		}

		inline Vector2 operator / (const Vector2& v) const
		{
			return Vector2(
				x / v.x,
				y / v.y);
		}

		inline const Vector2& operator + () const
		{
			return *this;
		}

		inline Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

		inline friend Vector2 operator * (const float32 scalar, const Vector2& v)
		{
			return Vector2(
				scalar * v.x,
				scalar * v.y);
		}

		inline friend Vector2 operator / (const float32 scalar, const Vector2& v)
		{
			return Vector2(
				scalar / v.x,
				scalar / v.y);
		}

		inline friend Vector2 operator + (const Vector2& v, const float32 val)
		{
			return Vector2(
				v.x + val,
				v.y + val);
		}

		inline friend Vector2 operator + (const float32 val, const Vector2& v)
		{
			return Vector2(
				val + v.x,
				val + v.y);
		}

		inline friend Vector2 operator - (const Vector2& v, const float32 val)
		{
			return Vector2(
				v.x - val,
				v.y - val);
		}

		inline friend Vector2 operator - (const float32 val, const Vector2& v)
		{
			return Vector2(
				val - v.x,
				val - v.y);
		}

		inline Vector2& operator += (const Vector2& v)
		{
			x += v.x;
			y += v.y;

			return *this;
		}

		inline Vector2& operator += (const float32 scalar)
		{
			x += scalar;
			y += scalar;

			return *this;
		}

		inline Vector2& operator -= (const Vector2& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		inline Vector2& operator -= (const float32 scalar)
		{
			x -= scalar;
			y -= scalar;

			return *this;
		}

		inline Vector2& operator *= (const float32 scalar)
		{
			x *= scalar;
			y *= scalar;

			return *this;
		}

		inline Vector2& operator *= (const Vector2& v)
		{
			x *= v.x;
			y *= v.y;

			return *this;
		}

		inline Vector2& operator /= (const float32 scalar)
		{
			assert(scalar != 0.0);

			float32 fInv = 1.0f / scalar;

			x *= fInv;
			y *= fInv;

			return *this;
		}

		inline Vector2& operator /= (const Vector2& v)
		{
			x /= v.x;
			y /= v.y;

			return *this;
		}

		inline float32 DotProduct(const Vector2& v) const
		{
			return x * v.x + y * v.y;
		}

		inline Vector2 MidPoint(const Vector2& v) const
		{
			return Vector2(
				(x + v.x) * 0.5f,
				(y + v.y) * 0.5f);
		}

		inline void MakeFloor(const Vector2& cmp)
		{
			x = cmp.x < x ? cmp.x : x;
			y = cmp.y < y ? cmp.y : y;
		}

		inline void MakeCeil(const Vector2& cmp)
		{
			x = cmp.x > x ? cmp.x : x;
			y = cmp.y > y ? cmp.y : y;
		}

		float32 GetLength() const;

		float32 GetSqrLength() const;

		float32 GetDistance(const Vector2& v) const;

		float32 GetSqrDistance(const Vector2& v) const;

		float32 CrossProduct(const Vector2& v) const;

		Vector2 Normalize();

		Vector2 Reflect(const Vector2& normal) const;

		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 ONE;
	};
}