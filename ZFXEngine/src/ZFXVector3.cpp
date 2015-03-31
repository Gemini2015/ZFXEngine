#include "ZFXVector3.h"

namespace ZFX
{

	const Vector3 Vector3::ORIGIN(0, 0, 0);
	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);
	const Vector3 Vector3::NEGATIVE_UNIT_X(-1, 0, 0);
	const Vector3 Vector3::NEGATIVE_UNIT_Y(0, -1, 0);
	const Vector3 Vector3::NEGATIVE_UNIT_Z(0, 0, -1);
	const Vector3 Vector3::ONE(1, 1, 1);


	float32 Vector3::GetLength() const
	{
		return (float32)sqrt(x * x + y * y + z * z);
	}

	float32 Vector3::GetSqrLength() const
	{
		return (x * x + y * y + z * z);
	}

	Vector3 Vector3::Normalize()
	{
		float64 f = sqrt(x * x + y * y + z * z);
		if (f != 0.0f)
		{
			f = 1 / f;
			return Vector3(x * f, y * f, z * f);
		}
		return Vector3(0, 0, 0);
	}

	Vector3 Vector3::VectorTo(const Vector3& v)
	{
		return Vector3(v.x - x, v.y - y, v.z - z);
	}

	float32 Vector3::GetDistance(const Vector3& v) const
	{
		return (*this - v).GetLength();
	}

	float32 Vector3::GetSqrDistance(const Vector3& v) const
	{
		return (*this - v).GetSqrLength();
	}

	Vector3 Vector3::CrossProduct(const Vector3& v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	Vector3 Vector3::Reflect(const Vector3& normal) const
	{
		return Vector3(*this - (2 * this->dotProduct(normal) * normal));
	}

}

