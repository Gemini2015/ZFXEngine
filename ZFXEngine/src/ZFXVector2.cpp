#include "ZFXVector2.h"

namespace ZFX
{
	const Vector2 Vector2::ZERO(0, 0);
	const Vector2 Vector2::UNIT_X(1, 0);
	const Vector2 Vector2::UNIT_Y(0, 1);
	const Vector2 Vector2::NEGATIVE_UNIT_X(-1, 0);
	const Vector2 Vector2::NEGATIVE_UNIT_Y(0, -1);
	const Vector2 Vector2::ONE(1, 1);

	float32 Vector2::GetLength() const
	{
		return (float32)sqrt(x * x + y * y);
	}

	float32 Vector2::GetSqrLength() const
	{
		return x * x + y * y;
	}

	float32 Vector2::GetDistance(const Vector2& v) const
	{
		return (*this - v).GetLength();
	}

	float32 Vector2::GetSqrDistance(const Vector2& v) const
	{
		return (*this - v).GetSqrLength();
	}

	float32 Vector2::CrossProduct(const Vector2& v) const
	{
		return x * v.y - y * v.x;
	}

	ZFX::Vector2 Vector2::Normalize()
	{
		float32 length = GetLength();
		if (length > 0.0f)
		{
			float32 fInv = 1.0f / length;
			x *= fInv;
			y *= fInv;
		}
		return *this;
	}

	ZFX::Vector2 Vector2::Reflect(const Vector2& normal) const
	{
		return Vector2(*this - (2 * this->DotProduct(normal) * normal));
	}

}