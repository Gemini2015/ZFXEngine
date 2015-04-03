#include "ZFXLight.h"

namespace ZFX
{

	Light::Light()
		: mLightType(Light::LT_POINT)
		, mName("")
		, mPosition(Vector3::ORIGIN)
		, mDiffuse(ColorValue::White)
		, mSpecular(ColorValue::Black)
		, mDirection(Vector3::UNIT_Z)
		, mSpotOuter(40.0f)
		, mSpotInner(30.0f)
		, mSpotFalloff(1.0f)
		, mSpotNearClip(0.0f)
		, mRange(100000)
		, mAttenuationConst(1.0f)
		, mAttenuationLinear(0.0f)
		, mAttenuationQuad(0.0f)
		, mPowerScale(1.0f)
	{

	}

	Light::Light(const String& name)
		: mLightType(Light::LT_POINT)
		, mName(name)
		, mPosition(Vector3::ORIGIN)
		, mDiffuse(ColorValue::White)
		, mSpecular(ColorValue::Black)
		, mDirection(Vector3::UNIT_Z)
		, mSpotOuter(40.0f)
		, mSpotInner(30.0f)
		, mSpotFalloff(1.0f)
		, mSpotNearClip(0.0f)
		, mRange(100000)
		, mAttenuationConst(1.0f)
		, mAttenuationLinear(0.0f)
		, mAttenuationQuad(0.0f)
		, mPowerScale(1.0f)
	{

	}

	Light::~Light()
	{

	}

	void Light::SetType(LightType type)
	{
		mLightType = type;
	}

	Light::LightType Light::GetType() const
	{
		return mLightType;
	}

	void Light::SetDiffuseColor(float32 red, float32 green, float32 blue)
	{
		mDiffuse = ColorValue(red, green, blue);
	}

	void Light::SetDiffuseColor(const ColorValue& color)
	{
		mDiffuse = color;
	}

	const ColorValue& Light::GetDiffuseColor() const
	{
		return mDiffuse;
	}

	void Light::SetSpecularColor(float32 red, float32 green, float32 blue)
	{
		mSpecular = ColorValue(red, green, blue);
	}

	void Light::SetSpecularColor(const ColorValue& color)
	{
		mSpecular = color;
	}

	const ColorValue& Light::GetSpecularColor() const
	{
		return mSpecular;
	}

	void Light::SetAttenuation(float32 range, float32 constant, float32 linear, float32 quad)
	{
		mRange = range;
		mAttenuationConst = constant;
		mAttenuationLinear = linear;
		mAttenuationQuad = quad;
	}

	ZFX::float32 Light::GetAttenuationRange() const
	{
		return mRange;
	}

	ZFX::float32 Light::GetAttenuationConstant() const
	{
		return mAttenuationConst;
	}

	ZFX::float32 Light::GetAttenuationLinear() const
	{
		return mAttenuationLinear;
	}

	ZFX::float32 Light::GetAttenuationQuad() const
	{
		return mAttenuationQuad;
	}

	void Light::SetPosition(float32 x, float32 y, float32 z)
	{
		mPosition = Vector3(x, y, z);
	}

	void Light::SetPosition(const Vector3& pos)
	{
		mPosition = pos;
	}

	const Vector3& Light::GetPosition() const
	{
		return mPosition;
	}

	void Light::SetDirection(float32 x, float32 y, float32 z)
	{
		mDirection = Vector3(x, y, z);
	}

	void Light::SetDirection(const Vector3& dir)
	{
		mDirection = dir;
	}

	const Vector3& Light::GetDirection() const
	{
		return mDirection;
	}

	void Light::SetSpotLightRange(const float32 innerAngle, const float32 outerAngle, float32 falloff /*= 1.0f*/)
	{
		mSpotInner = innerAngle;
		mSpotOuter = outerAngle;
		mSpotFalloff = falloff;
	}

	void Light::SetSpotLightInnerAngle(const float32 innerAngle)
	{
		mSpotInner = innerAngle;
	}

	ZFX::float32 Light::GetSpotLightInnerAngle() const
	{
		return mSpotInner;
	}

	void Light::SetSpotLightOuterAngle(const float32 OuterAngle)
	{
		mSpotOuter = OuterAngle;
	}

	ZFX::float32 Light::GetSpotLightOuterAngle() const
	{
		return mSpotOuter;
	}

	void Light::SetSpotLightFalloff(const float32 falloff)
	{
		mSpotFalloff = falloff;
	}

	ZFX::float32 Light::GetSpotLightFalloff() const
	{
		return mSpotFalloff;
	}

	void Light::SetSpotLightNearClipDistance(const float32 nearClip)
	{
		mSpotNearClip = nearClip;
	}

	ZFX::float32 Light::GetSpotLightNearClipDistance() const
	{
		return mSpotNearClip;
	}

	void Light::SetPowerScale(float32 power)
	{
		mPowerScale = power;
	}

	ZFX::float32 Light::GetPowerScale() const
	{
		return mPowerScale;
	}

}