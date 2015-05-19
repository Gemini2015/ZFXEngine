#pragma once

#include "ZFXPrerequisites.h"
#include "ZFXFrustum.h"

namespace ZFX
{
	class ZFXExport Camera: public Frustum
	{
	public:
		explicit Camera(const String& name);

		~Camera();

		void SetPolygonMode(PolygonMode mode);

		PolygonMode GetPolygonMode() const;

		void SetPosition(float32 x, float32 y, float32 z);

		void SetPosition(const Vector3& vec);

		Vector3 GetPosition() const;

		void Move(const Vector3& vec);

		void MoveRelative(const Vector3& vec);

		void SetDirection(float32 x, float32 y, float32 z);

		void SetDirection(const Vector3& vec);

		Vector3 GetDirection() const;

		Vector3 GetUp() const;

		Vector3 GetRight() const;

		void LookAt(float32 x, float32 y, float32 z);

		void LookAt(const Vector3& targetPoint);

		Viewport* GetViewport() const;

		void SetViewport(Viewport* vp);

		void Render(Viewport *vp);

	private:
		String mName;
		PolygonMode mPolygonMode;
		Vector3 mPosition;
		Vector3 mDirection;

		Viewport* mViewport;
	};
}