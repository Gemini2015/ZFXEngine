#ifndef _ZFX_PLANE_H_
#define _ZFX_PLANE_H_

#include "ZFXPrerequisites.h"

#include "ZFXVector3.h"

namespace ZFX
{
	class ZFXExport Plane
	{
	public:
		Vector3 normal;
		float32 d;
	public:
		Plane();

		Plane(const Plane& p);

		Plane(const Vector3& normal, float32 constant);

		Plane(float32 a, float32 b, float32 c, float32 d);

		Plane(const Vector3& normal, const Vector3& point);

		Plane(const Vector3& pa, const Vector3& pb, const Vector3& pc);

		float32 GetDistance(const Vector3& point) const;

		Vector3 ProjectVector(const Vector3& v) const;

		bool operator==(const Plane &p) const
		{
			return d == p.d && normal == p.normal;
		}
		bool operator!=(const Plane &p) const
		{
			return d != p.d || normal != p.normal;
		}
	};
}

#endif