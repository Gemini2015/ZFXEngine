#include "ZFXPlane.h"

namespace ZFX
{

	Plane::Plane()
	{
		normal = Vector3::ORIGIN;
		d = 0.0f;
	}

	Plane::Plane(const Plane& p)
	{
		normal = p.normal;
		d = p.d;
	}

	Plane::Plane(const Vector3& normal, float32 constant)
	{
		this->normal = normal;
		d = constant;
	}

	Plane::Plane(float32 a, float32 b, float32 c, float32 d)
		: normal(a, b, c), d(d)
	{
	}

	Plane::Plane(const Vector3& normal, const Vector3& point)
	{
		this->normal = normal;
		d = -normal.DotProduct(point);
	}

	Plane::Plane(const Vector3& pa, const Vector3& pb, const Vector3& pc)
	{
		Vector3 edge1 = pb - pa;
		Vector3 edge2 = pc - pa;
		normal = edge1.CrossProduct(edge2).Normalize();
		d = -normal.DotProduct(pa);
	}

	float32 Plane::GetDistance(const Vector3& point) const
	{
		return normal.DotProduct(point) + d;
	}

	ZFX::Vector3 Plane::ProjectVector(const Vector3& v) const
	{
		return Vector3::ORIGIN;
	}

}