#include "ZFXPlane.h"
#include "ZFXMatrix3.h"

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
		Matrix3 xform;
		xform[0][0] = 1.0f - normal.x * normal.x;
		xform[0][1] = -normal.x * normal.y;
		xform[0][2] = -normal.x * normal.z;
		xform[1][0] = -normal.y * normal.x;
		xform[1][1] = 1.0f - normal.y * normal.y;
		xform[1][2] = -normal.y * normal.z;
		xform[2][0] = -normal.z * normal.x;
		xform[2][1] = -normal.z * normal.y;
		xform[2][2] = 1.0f - normal.z * normal.z;

		return xform * v;
	}

	Plane::Side Plane::GetSide(const Vector3& vertex) const
	{
		float32 distance = GetDistance(vertex);
		if (distance < 0.0f)
			return Plane::NEGATIVE_SIDE;
		if (distance > 0.0f)
			return Plane::POSITIVE_SIDE;
		return Plane::NO_SIDE;
	}

}