#pragma once

#include "ZFXPrerequisites.h"
#include "ZFXVector2.h"
#include "ZFXVector4.h"
#include "ZFXPlane.h"
#include "ZFXMatrix4.h"

namespace ZFX
{
	enum ProjectionType
	{
		PT_ORTHOGRAPHIC,
		PT_PERSPECTIVE
	};

	enum FrustumPlane
	{
		FP_NEAR = 0,
		FP_FAR = 1,
		FP_LEFT = 2,
		FP_TOP = 3,
		FP_RIGHT = 4,
		FP_BOTTOM = 5,
		FP_MAX_NUM
	};

	class ZFXExport Frustum
	{
	protected:
		ProjectionType mProjType;

		float32 mFOVy;
		float32 mFarDist;
		float32 mNearDist;
		float32 mAspect;
		float32 mOrthoHeight;
		Vector2 mFrustumOffset;
		float32 mFocalLength;

		mutable Plane mFrustumPlanes[FP_MAX_NUM];

		mutable Matrix4 mProjMatrixForRS;
		mutable Matrix4 mProjMatrixForRSWithDepth;
		mutable Matrix4 mProjMatrix;
		mutable Matrix4 mViewMatrix;

		mutable bool mRecalcFrustum;
		mutable bool mRecalcView;
		mutable bool mRecalcFrustumPlanes;
		mutable bool mRecalcWorldSpaceCorners;

		bool mCustomViewMatrix;
		bool mCustomProjMatrix;
		bool mFrustumExtentsManuallySet;
		float32 mLeft, mTop, mRight, mBottom;

		static const float32 INFINITE_FAR_PLANE_ADJUST;

	public:
		Frustum();

		~Frustum();

		void SetFOVy(float32 fovy);

		const float32 GetFOVy() const;

		void SetNearClipDistance(float32 nearDist);

		float32 GetNearClipDistance() const;

		void SetFarClipDistance(float32 farDist);

		float32 GetFarClipDistance() const;

		void SetAspectRatio(float32 ratio);

		float32 GetAspectRatio() const;

		void SetFrustumOffset(const Vector2& offset);

		void SetFrustumOffset(float32 h = 0.0f, float32 v = 0.0f);

		const Vector2& GetFrustumOffset() const;

		void SetFrustumExtents(float32 left, float32 top, float32 right, float32 bottom);

		void ResetFrustumExtents();

		const Vector4 GetFrustumExtents();

		const Matrix4& GetProjMatrixForRS() const;

		const Matrix4& GetProjMatrixForRSWithDepth() const;

		const Matrix4& GetProjMatrix() const;

		const Matrix4& GetViewMatrix() const;

		void SetCustomViewMatrix(bool enable, const Matrix4& viewMat = Matrix4::IDENTITY);

		bool IsCustomViewMatrixEnabled() const { return mCustomViewMatrix; }

		void SetCustomProjMatrix(bool enable, const Matrix4& projMat = Matrix4::IDENTITY);

		bool IsCustomProjMatrixEnabled() const { return mCustomProjMatrix; }

		const Plane* GetFrustumPlanes() const;

		const Plane& GetFrustumPlane(uint32 plane) const;

		bool IsVisible(const Vector3& vertex, FrustumPlane* cullplanes = nullptr) const;

		void SetProjType(ProjectionType type);

		ProjectionType GetProjType() const;

		void SetOrthoWindow(float32 width, float32 height);

		void SetOrthoWindowWidth(float32 width);

		void SetOrthoWindowHeight(float32 height);

		const Vector2 GetOrthoWindow() const;

	protected:
		const Vector4 GetProjParameters() const;

		void UpdateFrustum() const;

		void UpdateView() const;

		void UpdateFrustumPlanes() const;

		bool IsFrustumOutOfDate() const;

		bool IsViewOutOfDate() const;

		void InvalidateFrustum();

		void InvalidateView();
	};
}