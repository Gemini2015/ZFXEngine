#ifndef _ZFX_PLANE_H_
#define _ZFX_PLANE_H_

namespace ZFX
{
	class Plane
	{
	public:
		bool operator==(const Plane &p) const
		{
			return true;
		}
		bool operator!=(const Plane &p) const
		{
			return false;
		}
	};
}

#endif