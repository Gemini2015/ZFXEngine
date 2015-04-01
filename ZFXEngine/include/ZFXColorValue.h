#ifndef _ZFX_COLOR_VALUE_H_
#define _ZFX_COLOR_VALUE_H_

#include "ZFXPrerequisites.h"

namespace ZFX
{
	typedef uint32 RGBA;
	typedef uint32 ARGB;
	typedef uint32 ABGR;
	typedef uint32 BGRA;

	class ColorValue
	{
	public:
		union
		{
			struct
			{
				float32 r;
				float32 g;
				float32 b;
				float32 a;
			};
			float32 val[4];
		};

	public:
		ColorValue(
			float32 red = 1.0f,
			float32 green = 1.0f,
			float32 blue = 1.0f,
			float32 alpha = 1.0f)
			: r(red), g(green), b(blue), a(alpha)
		{
		}

		ColorValue(float32 scalar)
		{
			r = g = b = a = scalar;
		}

		RGBA GetAsRGBA() const;
		void SetAsRGBA(const RGBA val);

		ARGB GetAsARGB() const;
		void SetAsARGB(const ARGB val);

		BGRA GetAsBGRA() const;
		void SetAsBGRA(const BGRA val);

		ABGR GetAsABGR() const;
		void SetAsABGR(const ABGR val);

		void Clamp();
		ColorValue GetAsClamp() const;

		inline bool operator==(const ColorValue& c) const
		{
			return r == c.r && g == c.g && b == c.b && a == c.a;
		}

		inline bool operator!=(const ColorValue& c) const
		{
			return r != c.r || g != c.g || b != c.b || a != c.a;
		}

		inline float32 operator[](const size_t i) const
		{
			assert(i < 4);
			return val[i];
		}

		inline float32& operator[](const size_t i)
		{
			assert(i < 4);
			return val[i];
		}

		inline ColorValue operator+(const ColorValue& c) const
		{
			ColorValue ret;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = val[i] + c.val[i];
			return ret;
		}

		inline ColorValue operator-(const ColorValue& c) const
		{
			ColorValue ret;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = val[i] - c.val[i];
			return ret;
		}

		inline ColorValue operator*(const float32 scalar) const
		{
			ColorValue ret;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = val[i] * scalar;
			return ret;
		}

		inline ColorValue operator*(const ColorValue& c) const
		{
			ColorValue ret;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = val[i] * c.val[i];
			return ret;
		}

		inline ColorValue operator/(const float32 scalar) const
		{
			assert(scalar != 0.0f);
			ColorValue ret;
			float32 finv = 1.0f / scalar;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = val[i] * finv;
			return ret;
		}

		inline friend ColorValue operator*(const float32 scalar, const ColorValue& c)
		{
			ColorValue ret;
			for (size_t i = 0; i < 4; i++)
				ret.val[i] = c.val[i] * scalar;
			return ret;
		}

		inline ColorValue& operator+=(const ColorValue& c)
		{
			for (size_t i = 0; i < 4; i++)
				val[i] = val[i] + c.val[i];
			return *this;
		}

		inline ColorValue& operator-=(const ColorValue& c)
		{
			for (size_t i = 0; i < 4; i++)
				val[i] = val[i] - c.val[i];
			return *this;
		}

		inline ColorValue& operator*=(const float32 scalar)
		{
			for (size_t i = 0; i < 4; i++)
				val[i] = val[i] + scalar;
			return *this;
		}

		inline ColorValue& operator/=(const float32 scalar)
		{
			assert(scalar != 0.0f);
			float32 finv = 1.0f / scalar;
			for (size_t i = 0; i < 4; i++)
				val[i] = val[i] * scalar;
			return *this;
		}

	};
}

#endif