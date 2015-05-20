#include "ZFXMath.h"

namespace ZFX
{

	const float32 Math::PI = float32( 4.0f * atan(1.0f));

	const float32 Math::TWO_PI = float32( 2.0f * PI);

	const float32 Math::HALF_PI = float32( 0.5f * PI);

	const float32 Math::Deg2Rad = PI / 180.0f;

	const float32 Math::Rad2Deg = 180.0f / PI;


	float32 Radian::ToDegrees() const
	{
		return Math::Rad2Deg * val;
	}


	float32 Degree::ToRadians() const
	{
		return Math::Deg2Rad * val;
	}

}