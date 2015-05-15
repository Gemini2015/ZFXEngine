#include "ZFXFrustum.h"

namespace ZFX
{
	const float32 Frustum::INFINITE_FAR_PLANE_ADJUST = 0.00001f;

	Frustum::Frustum()
		: mProjType(PT_PERSPECTIVE)
		, mFOVy(45.0f)
		, mFarDist(100000.0f)
		, mNearDist(100.0f)
		, mAspect(1.333333333f)
		, mOrthoHeight(1000)
		, mFrustumOffset(Vector2::ZERO)
		, mFocalLength(1.0f)
		, mRecalcFrustum(true)
		, mRecalcView(true)
		, mRecalcFrustumPlanes(true)
		, mRecalcWorldSpaceCorners(true)
		, mCustomViewMatrix(false)
		, mCustomProjMatrix(false)
	{
		UpdateView();
		UpdateFrustum();
	}

	Frustum::~Frustum()
	{

	}

	void Frustum::SetFOVy(float32 fovy)
	{
		mFOVy = fovy;
		InvalidateFrustum();
	}

	const float32 Frustum::GetFOVy() const
	{
		return mFOVy;
	}

	void Frustum::SetNearClipDistance(float32 nearDist)
	{
		mNearDist = nearDist;
		InvalidateFrustum();
	}

	float32 Frustum::GetNearClipDistance() const
	{
		return mNearDist;
	}

	void Frustum::SetFarClipDistance(float32 farDist)
	{
		mFarDist = farDist;
		InvalidateFrustum();
	}

	float32 Frustum::GetFarClipDistance() const
	{
		return mFarDist;
	}

	void Frustum::SetAspectRatio(float32 ratio)
	{
		mAspect = ratio;
		InvalidateFrustum();
	}

	float32 Frustum::GetAspectRatio() const
	{
		return mAspect;
	}

	void Frustum::SetFrustumOffset(const Vector2& offset)
	{
		mFrustumOffset = offset;
		InvalidateFrustum();
	}

	void Frustum::SetFrustumOffset(float32 h /*= 0.0f*/, float32 v /*= 0.0f*/)
	{
		mFrustumOffset.x = h;
		mFrustumOffset.y = v;
		InvalidateFrustum();
	}

	const Vector2& Frustum::GetFrustumOffset() const
	{
		return mFrustumOffset;
	}

	void Frustum::SetFrustumExtents(float32 left, float32 top, float32 right, float32 bottom)
	{
		mFrustumExtentsManuallySet = true;
		mLeft = left;
		mTop = top;
		mRight = right;
		mBottom = bottom;

		InvalidateFrustum();
	}

	void Frustum::ResetFrustumExtents()
	{
		mFrustumExtentsManuallySet = false;
		InvalidateFrustum();
	}

	const Vector4 Frustum::GetFrustumExtents()
	{
		return Vector4(mLeft, mTop, mRight, mBottom);
	}

	const Matrix4& Frustum::GetProjMatrixForRS() const
	{
		UpdateFrustum();
		return mProjMatrixForRS;
	}

	const Matrix4& Frustum::GetProjMatrixForRSWithDepth() const
	{
		UpdateFrustum();
		return mProjMatrixForRSWithDepth;
	}

	const Matrix4& Frustum::GetProjMatrix() const
	{
		UpdateFrustum();
		return mProjMatrix;
	}

	const Matrix4& Frustum::GetViewMatrix() const
	{
		UpdateView();
		return mViewMatrix;
	}

	void Frustum::SetCustomViewMatrix(bool enable, const Matrix4& viewMat /*= Matrix4::IDENTITY*/)
	{
		mCustomViewMatrix = enable;
		if (enable)
		{
			assert(viewMat.IsAffine());
			mViewMatrix = viewMat;
		}
		InvalidateView();
	}

	void Frustum::SetCustomProjMatrix(bool enable, const Matrix4& projMat /*= Matrix4::IDENTITY*/)
	{
		mCustomProjMatrix = enable;
		if (enable)
		{
			mProjMatrix = projMat;
		}
		InvalidateFrustum();
	}

	const Plane* Frustum::GetFrustumPlanes() const
	{
		return mFrustumPlanes;
	}

	const Plane& Frustum::GetFrustumPlane(uint32 plane) const
	{
		assert(plane < FP_MAX_NUM);
		return mFrustumPlanes[plane];
	}

	bool Frustum::IsVisible(const Vector3& vertex, FrustumPlane* cullplanes /*= nullptr*/) const
	{
		UpdateFrustumPlanes();

		for (int plane = 0; plane < FP_MAX_NUM; ++plane)
		{
			if(plane == FP_FAR && mFarDist == 0)
				continue;

			if (mFrustumPlanes[plane].GetSide(vertex) == Plane::NEGATIVE_SIDE)
			{
				if (cullplanes)
					*cullplanes = (FrustumPlane)plane;
				return false;
			}
		}
		return true;
	}

	void Frustum::SetProjType(ProjectionType type)
	{
		mProjType = type;
		InvalidateFrustum();
	}

	ZFX::ProjectionType Frustum::GetProjType() const
	{
		return mProjType;
	}

	void Frustum::SetOrthoWindow(float32 width, float32 height)
	{
		assert(height != 0.0f);
		mOrthoHeight = height;
		mAspect = width / height;
		InvalidateFrustum();
	}

	void Frustum::SetOrthoWindowWidth(float32 width)
	{
		mOrthoHeight = width / mAspect;
		InvalidateFrustum();
	}

	void Frustum::SetOrthoWindowHeight(float32 height)
	{
		mOrthoHeight = height;
		InvalidateFrustum();
	}

	const Vector2 Frustum::GetOrthoWindow() const
	{
		return Vector2(mOrthoHeight * mAspect, mOrthoHeight);
	}

	const Vector4 Frustum::GetProjParameters() const
	{
		Vector4 ret;
		if (mCustomProjMatrix)
		{
			Matrix4 invProj = mProjMatrix.Inverse();
			Vector3 topleft(-0.5, 0.5f, 0.0f);
			Vector3 bottomright(0.5f, -0.5f, 0.0f);

			topleft = invProj * topleft;
			bottomright = invProj * bottomright;

			ret.left = topleft.x;
			ret.top = topleft.y;
			ret.right = bottomright.x;
			ret.bottom = bottomright.y;
		}
		else
		{
			if (mFrustumExtentsManuallySet)
			{
				ret = Vector4(mLeft, mTop, mRight, mBottom);
			}
			else if(mProjType == PT_PERSPECTIVE)
			{
				float32 thetaV = (mFOVy / 180.0f) * (atan(1.0f) * 4) / 2.0f;
				float32 tanThetaV = tan(thetaV);
				float32 tanThetaH = tanThetaV * mAspect;

				float32 nearFocal = mNearDist / mFocalLength;
				float32 nearOffsetX = mFrustumOffset.x * nearFocal;
				float32 nearOffsetY = mFrustumOffset.y * nearFocal;
				float32 halfWidth = tanThetaH * mNearDist;
				float32 halfHeight = tanThetaV * mNearDist;

				ret = Vector4(-halfWidth + nearOffsetX, halfHeight + nearOffsetY,
					halfWidth + nearOffsetX, -halfHeight + nearOffsetY);

			}
			else
			{
				Vector2 size = GetOrthoWindow();
				float32 halfWidth = size.width * 0.5f;
				float32 halfHeight = size.height * 0.5f;

				ret = Vector4(-halfWidth, halfHeight, halfWidth, -halfHeight);
			}
		}
		return ret;
	}

	void Frustum::UpdateFrustum() const
	{
		if (!IsFrustumOutOfDate())
			return;

		Vector4 rect = GetProjParameters();
		if (!mCustomProjMatrix)
		{
			float32 invWidth = 1 / (rect.right - rect.left);
			float32 invHeight = 1 / (rect.top - rect.bottom);
			float32 invDistance = 1 / (mFarDist - mNearDist);

			if (mProjType == PT_PERSPECTIVE)
			{
				float32 A = 2 * mNearDist * invWidth;
				float32 B = 2 * mNearDist * invHeight;
				float32 C = (rect.right + rect.left) * invWidth;
				float32 D = (rect.top + rect.bottom) * invHeight;
				float32 q, qn;

				if (mFarDist == 0)
				{
					q = Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
					qn = mNearDist * (Frustum::INFINITE_FAR_PLANE_ADJUST - 2);
				}
				else
				{
					q = -(mFarDist + mNearDist) * invDistance;
					qn = -2 * (mFarDist * mNearDist) * invDistance;
				}

				mProjMatrix = Matrix4::ZERO;
				mProjMatrix[0][0] = A;
				mProjMatrix[0][2] = C;
				mProjMatrix[1][1] = B;
				mProjMatrix[1][2] = D;
				mProjMatrix[2][2] = q;
				mProjMatrix[2][3] = qn;
				mProjMatrix[3][2] = -1;
			}
			else
			{
				float32 A = 2 * invWidth;
				float32 B = 2 * invHeight;
				float32 C = -(rect.right + rect.left) * invWidth;
				float32 D = -(rect.top + rect.bottom) * invHeight;
				float32 q, qn;

				if (mFarDist == 0)
				{
					q = -Frustum::INFINITE_FAR_PLANE_ADJUST / mNearDist;
					qn = -Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
				}
				else
				{
					q = -2 * invDistance;
					qn = -(mFarDist + mNearDist) * invDistance;
				}

				mProjMatrix = Matrix4::ZERO;
				mProjMatrix[0][0] = A;
				mProjMatrix[0][3] = C;
				mProjMatrix[1][1] = B;
				mProjMatrix[1][3] = D;
				mProjMatrix[2][2] = q;
				mProjMatrix[2][3] = qn;
				mProjMatrix[3][3] = 1;
			}
		}

		mRecalcFrustum = false;
		mRecalcFrustumPlanes = true;
	}

	void Frustum::UpdateView() const
	{
		if (!IsViewOutOfDate())
			return;

	}

	void Frustum::UpdateFrustumPlanes() const
	{
		UpdateView();
		UpdateFrustum();
		if (!mRecalcFrustumPlanes)
			return;

		Matrix4 combo = mProjMatrix * mViewMatrix;

		mFrustumPlanes[FP_LEFT].normal.x = combo[3][0] + combo[0][0];
		mFrustumPlanes[FP_LEFT].normal.y = combo[3][1] + combo[0][1];
		mFrustumPlanes[FP_LEFT].normal.z = combo[3][2] + combo[0][2];
		mFrustumPlanes[FP_LEFT].d = combo[3][3] + combo[0][3];

		mFrustumPlanes[FP_RIGHT].normal.x = combo[3][0] - combo[0][0];
		mFrustumPlanes[FP_RIGHT].normal.y = combo[3][1] - combo[0][1];
		mFrustumPlanes[FP_RIGHT].normal.z = combo[3][2] - combo[0][2];
		mFrustumPlanes[FP_RIGHT].d = combo[3][3] - combo[0][3];

		mFrustumPlanes[FP_TOP].normal.x = combo[3][0] - combo[1][0];
		mFrustumPlanes[FP_TOP].normal.y = combo[3][1] - combo[1][1];
		mFrustumPlanes[FP_TOP].normal.z = combo[3][2] - combo[1][2];
		mFrustumPlanes[FP_TOP].d = combo[3][3] - combo[1][3];

		mFrustumPlanes[FP_BOTTOM].normal.x = combo[3][0] + combo[1][0];
		mFrustumPlanes[FP_BOTTOM].normal.y = combo[3][1] + combo[1][1];
		mFrustumPlanes[FP_BOTTOM].normal.z = combo[3][2] + combo[1][2];
		mFrustumPlanes[FP_BOTTOM].d = combo[3][3] + combo[1][3];

		mFrustumPlanes[FP_NEAR].normal.x = combo[3][0] + combo[2][0];
		mFrustumPlanes[FP_NEAR].normal.y = combo[3][1] + combo[2][1];
		mFrustumPlanes[FP_NEAR].normal.z = combo[3][2] + combo[2][2];
		mFrustumPlanes[FP_NEAR].d = combo[3][3] + combo[2][3];

		mFrustumPlanes[FP_FAR].normal.x = combo[3][0] - combo[2][0];
		mFrustumPlanes[FP_FAR].normal.y = combo[3][1] - combo[2][1];
		mFrustumPlanes[FP_FAR].normal.z = combo[3][2] - combo[2][2];
		mFrustumPlanes[FP_FAR].d = combo[3][3] - combo[2][3];

		// Renormalise any normals which were not unit length
		for (int i = 0; i < 6; i++)
		{
			float32 length = mFrustumPlanes[i].normal.GetLength();
			mFrustumPlanes[i].normal.Normalize();
			mFrustumPlanes[i].d /= length;
		}

		mRecalcFrustumPlanes = false;

	}

	bool Frustum::IsFrustumOutOfDate() const
	{
		if (IsViewOutOfDate())
		{
			mRecalcFrustum = true;
		}
		return mRecalcFrustum;
	}

	bool Frustum::IsViewOutOfDate() const
	{
		return mRecalcView;
	}

	void Frustum::InvalidateFrustum()
	{
		mRecalcFrustum = true;
		mRecalcFrustumPlanes = true;
	}

	void Frustum::InvalidateView()
	{
		mRecalcView = true;
		mRecalcFrustumPlanes = true;
	}

}