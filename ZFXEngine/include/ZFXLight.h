#ifndef _ZFX_LIGHT_H_
#define _ZFX_LIGHT_H_

#include "ZFXPrerequisites.h"
#include "ZFXVector3.h"
#include "ZFXColorValue.h"

namespace ZFX
{
	class ZFXExport Light
	{
	public:
		enum LightType
		{
			LT_POINT,
			LT_DIRECTIONAL,
			LT_SPOT_LIGHT,
		};

	public:
		Light();

		explicit Light(const String& name);

		~Light();

		void SetType(LightType type);

		LightType GetType() const;

		void SetDiffuseColor(float32 red, float32 green, float32 blue);

		void SetDiffuseColor(const ColorValue& color);

		const ColorValue& GetDiffuseColor() const;

		void SetSpecularColor(float32 red, float32 green, float32 blue);

		void SetSpecularColor(const ColorValue& color);

		const ColorValue& GetSpecularColor() const;

		void SetAttenuation(float32 range, float32 constant, float32 linear, float32 quad);

		float32 GetAttenuationRange() const;

		float32 GetAttenuationConstant() const;

		float32 GetAttenuationLinear() const;

		float32 GetAttenuationQuad() const;

		void SetPosition(float32 x, float32 y, float32 z);

		void SetPosition(const Vector3& pos);

		const Vector3& GetPosition() const;

		void SetDirection(float32 x, float32 y, float32 z);

		void SetDirection(const Vector3& dir);

		const Vector3& GetDirection() const;

		void SetSpotLightRange(const float32 innerAngle, const float32 outerAngle, float32 falloff = 1.0f);

		void SetSpotLightInnerAngle(const float32 innerAngle);

		float32 GetSpotLightInnerAngle() const;

		void SetSpotLightOuterAngle(const float32 OuterAngle);

		float32 GetSpotLightOuterAngle() const;

		void SetSpotLightFalloff(const float32 falloff);

		float32 GetSpotLightFalloff() const;

		void SetSpotLightNearClipDistance(const float32 nearClip);

		float32 GetSpotLightNearClipDistance() const;

		void SetPowerScale(float32 power);

		float32 GetPowerScale() const;

	protected:
		LightType mLightType;
		String mName;
		Vector3 mPosition;
		ColorValue mDiffuse;
		ColorValue mSpecular;

		Vector3 mDirection;

		float32 mSpotOuter;
		float32 mSpotInner;
		float32 mSpotFalloff;
		float32 mSpotNearClip;
		float32 mRange;
		float32 mAttenuationConst;
		float32 mAttenuationLinear;
		float32 mAttenuationQuad;
		float32 mPowerScale;

	};
}

#endif