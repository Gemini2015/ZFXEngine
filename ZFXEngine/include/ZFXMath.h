#pragma once

#include "ZFXPrerequisites.h"

namespace ZFX
{
	class Radian
	{
	public:
		explicit Radian(float32 r = 0.0f)
			: val(r)
		{
		}

		inline Radian(const Radian& r)
		{
			val = r.val;
		}

		float32 ToDegrees() const;

		float32 ToRadians() const
		{
			return val;
		}

	private:
		float32 val;
	};

	class Degree
	{
	public:
		explicit Degree(float32 d = 0.0f)
			: val(d)
		{
		}

		inline Degree(const Degree& d)
		{
			val = d.val;
		}

		float32 ToDegrees() const
		{
			return val;
		}

		float32 ToRadians() const;

	private:
		float32 val;
	};

	class Angle
	{
	public:
		explicit Angle(float32 angle)
			: val(angle)
		{
		}

	private:
		float32 val;
	};

	class ZFXExport Math
	{
	public:
		enum AngleUnit
		{
			AU_DEGREE,
			AU_RADIAN
		};
	public:
		Math() { }

		~Math() { }

		static const float32 PI;

		static const float32 TWO_PI;

		static const float32 HALF_PI;

		static const float32 Deg2Rad;

		static const float32 Rad2Deg;
	};
}