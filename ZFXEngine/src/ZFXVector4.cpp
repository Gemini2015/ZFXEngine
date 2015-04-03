#include "ZFXVector4.h"

namespace ZFX
{
	const Vector4 Vector4::ZERO(0, 0, 0, 0);
	const Vector4 Vector4::ONE(1, 1, 1, 1);

	float32 Vector4::GetLength()
	{
		return (float32)sqrt(x * x + y * y + z * z);
	}

	float32 Vector4::GetSqrLength()
	{
		return (x * x + y * y + z * z);
	}

	Vector4 Vector4::Normalize()
	{
		float32 f = sqrt(x * x + y * y + z * z);
		if (f != 0.0f)
		{
			f = 1 / f;
			return Vector4(x * f, y * f, z * f, w);
		}
		return Vector4(0, 0, 0, 0);
	}

	Vector4 Vector4::VectorTo(const Vector4& v)
	{
		return Vector4(v.x - x, v.y - y, v.z - z);
	}

}