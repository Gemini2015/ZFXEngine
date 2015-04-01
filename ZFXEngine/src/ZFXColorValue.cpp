#include "ZFXColorValue.h"

namespace ZFX
{


	ZFX::RGBA ColorValue::GetAsRGBA() const
	{
		uint32 ret = 0x00000000;
		uint8 red = static_cast<uint8>(r * 255);
		ret |= red << 24;

		uint8 green = static_cast<uint8>(g * 255);
		ret |= green << 16;

		uint8 blue = static_cast<uint8>(b * 255);
		ret |= blue << 8;

		uint8 alpha = static_cast<uint8>(a * 255);
		ret |= alpha;

		return ret;
	}

	void ColorValue::SetAsRGBA(const RGBA val)
	{
		r = ((val >> 24) & 0xff) / 255.0f;
		g = ((val >> 16) & 0xff) / 255.0f;
		b = ((val >> 8) & 0xff) / 255.0f;
		a = ((val) & 0xff) / 255.0f;
	}

	ARGB ColorValue::GetAsARGB() const
	{
		uint32 ret = 0x00000000;
		uint8 red = static_cast<uint8>(r * 255);
		ret |= red << 16;

		uint8 green = static_cast<uint8>(g * 255);
		ret |= green << 8;

		uint8 blue = static_cast<uint8>(b * 255);
		ret |= blue;

		uint8 alpha = static_cast<uint8>(a * 255);
		ret |= alpha << 24;

		return ret;
	}

	void ColorValue::SetAsARGB(const ARGB val)
	{
		r = ((val >> 16) & 0xff) / 255.0f;
		g = ((val >> 8) & 0xff) / 255.0f;
		b = ((val) & 0xff) / 255.0f;
		a = ((val >> 24)& 0xff) / 255.0f;
	}

	ZFX::BGRA ColorValue::GetAsBGRA() const
	{
		uint32 ret = 0x00000000;
		uint8 red = static_cast<uint8>(r * 255);
		ret |= red << 8;

		uint8 green = static_cast<uint8>(g * 255);
		ret |= green << 16;

		uint8 blue = static_cast<uint8>(b * 255);
		ret |= blue << 24;

		uint8 alpha = static_cast<uint8>(a * 255);
		ret |= alpha;

		return ret;
	}

	void ColorValue::SetAsBGRA(const BGRA val)
	{
		r = ((val >> 8) & 0xff) / 255.0f;
		g = ((val >> 16) & 0xff) / 255.0f;
		b = ((val >> 24) & 0xff) / 255.0f;
		a = ((val)& 0xff) / 255.0f;
	}

	ZFX::ABGR ColorValue::GetAsABGR() const
	{
		uint32 ret = 0x00000000;
		uint8 red = static_cast<uint8>(r * 255);
		ret |= red;

		uint8 green = static_cast<uint8>(g * 255);
		ret |= green << 8;

		uint8 blue = static_cast<uint8>(b * 255);
		ret |= blue << 16;

		uint8 alpha = static_cast<uint8>(a * 255);
		ret |= alpha << 24;

		return ret;
	}

	void ColorValue::SetAsABGR(const ABGR val)
	{
		r = ((val) & 0xff) / 255.0f;
		g = ((val >> 8) & 0xff) / 255.0f;
		b = ((val >> 16) & 0xff) / 255.0f;
		a = ((val >> 24)& 0xff) / 255.0f;
	}

	void ColorValue::Clamp()
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (val[i] < 0.0f)
				val[i] = 0.0f;
			else if (val[i] > 1.0f)
				val[i] = 1.0f;
		}
	}

	ZFX::ColorValue ColorValue::GetAsClamp() const
	{
		ColorValue ret = *this;
		ret.Clamp();
		return ret;
	}

}