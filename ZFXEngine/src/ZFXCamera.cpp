#include "ZFXCamera.h"

namespace ZFX
{
	Camera::Camera(const String& name)
		: mName(name)
		, mPolygonMode(PM_SOLID)
		, mPosition(Vector3::ORIGIN)
		, mDirection(Vector3::ONE)
		, mViewport(nullptr)
	{
		mViewMatrix = Matrix4::ZERO;
		mProjMatrix = Matrix4::ZERO;
	}

	Camera::~Camera()
	{

	}

	void Camera::SetPolygonMode(PolygonMode mode)
	{
		mPolygonMode = mode;
	}

	ZFX::PolygonMode Camera::GetPolygonMode() const
	{
		return mPolygonMode;
	}

	void Camera::SetPosition(float32 x, float32 y, float32 z)
	{
		SetPosition(Vector3(x, y, z));
	}

	void Camera::SetPosition(const Vector3& vec)
	{
		mPosition = vec;
		InvalidateView();
	}

	ZFX::Vector3 Camera::GetPosition() const
	{
		return mPosition;
	}

	void Camera::Move(const Vector3& vec)
	{
		mPosition = mPosition + vec;
		InvalidateView();
	}

	void Camera::MoveRelative(const Vector3& vec)
	{
		throw std::logic_error("todo");
		InvalidateView();
	}

	void Camera::SetDirection(float32 x, float32 y, float32 z)
	{
		SetDirection(Vector3(x, y, z));
	}

	void Camera::SetDirection(const Vector3& vec)
	{
		if (vec == Vector3::ORIGIN)
			return;
		mDirection = vec;
		InvalidateView();
	}

	ZFX::Vector3 Camera::GetDirection() const
	{
		return mDirection;
	}

	ZFX::Vector3 Camera::GetUp() const
	{
		throw std::logic_error("todo");
	}

	ZFX::Vector3 Camera::GetRight() const
	{
		throw std::logic_error("todo");
	}

	void Camera::LookAt(float32 x, float32 y, float32 z)
	{

	}

	void Camera::LookAt(const Vector3& targetPoint)
	{

	}

	Viewport* Camera::GetViewport() const
	{
		return mViewport;
	}

	void Camera::SetViewport(Viewport* vp)
	{
		mViewport = vp;
	}

	void Camera::Render(Viewport *vp)
	{

	}

}