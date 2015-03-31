#include "ZFXMatrix3.h"
#include "ZFXVector3.h"

namespace ZFX
{
	const Matrix3 Matrix3::ZERO(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0);

	const Matrix3 Matrix3::IDENTITY(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);

	ZFX::Vector3 Matrix3::GetColumn(size_t col) const
	{
		assert(col < 3);
		return Vector3(m[0][col], m[1][col], m[2][col]);
	}

	void Matrix3::SetColumn(size_t col, const Vector3& v)
	{
		assert(col < 3);
		m[0][col] = v.x;
		m[1][col] = v.y;
		m[2][col] = v.z;
	}

	bool Matrix3::operator==(const Matrix3& mat) const
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (val[i] != mat.val[i])
				return false;
		}
		return true;
	}

	bool Matrix3::operator!=(const Matrix3& mat) const
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (val[i] == mat.val[i])
				return false;
		}
		return true;
	}

	ZFX::Matrix3 Matrix3::operator+(const Matrix3& mat) const
	{
		Matrix3 ret;
		for (size_t i = 0; i < 9; i++)
		{
			ret.val[i] = val[i] + mat.val[i];
		}
		return ret;
	}

	ZFX::Matrix3 Matrix3::operator-(const Matrix3& mat) const
	{
		Matrix3 ret;
		for (size_t i = 0; i < 9; i++)
		{
			ret.val[i] = val[i] - mat.val[i];
		}
		return ret;
	}

	ZFX::Matrix3 Matrix3::operator*(const Matrix3& mat) const
	{
		Matrix3 ret;
		for (size_t row = 0; row < 3; row++)
		{
			for (size_t col = 0; col < 3; col++)
			{
				ret.m[row][col] =
					m[row][0] * mat.m[0][col] +
					m[row][1] * mat.m[1][col] +
					m[row][2] * mat.m[2][col];
			}
		}
		return ret;
	}

	ZFX::Vector3 Matrix3::operator*(const Vector3& v) const
	{
		Vector3 ret;
		for (size_t row = 0; row < 3; row++)
		{
			ret[row] =
				m[row][0] * v[0] +
				m[row][1] * v[1] +
				m[row][2] * v[2];
		}
		return ret;
	}

	ZFX::Vector3 operator*(const Vector3& v, const Matrix3& mat)
	{
		Vector3 ret;
		for (size_t row = 0; row < 3; row++)
		{
			ret[row] =
				v[0] * mat.m[0][row] +
				v[1] * mat.m[1][row] +
				v[2] * mat.m[2][row];
		}
		return ret;
	}

	ZFX::Matrix3 Matrix3::operator*(float32 scalar) const
	{
		Matrix3 ret;
		for (size_t i = 0; i < 9; i++)
		{
			ret.val[i] = val[i] * scalar;
		}
		return ret;
	}

	ZFX::Matrix3 operator*(float32 scalar, const Matrix3& mat)
	{
		Matrix3 ret;
		for (size_t i = 0; i < 9; i++)
		{
			ret.val[i] = scalar * mat.val[i];
		}
		return ret;
	}

	ZFX::Matrix3 Matrix3::operator-() const
	{
		Matrix3 ret;
		for (size_t i = 0; i < 9; i++)
		{
			ret.val[i] = -val[i];
		}
		return ret;
	}

	ZFX::Matrix3 Matrix3::Transpose() const
	{
		Matrix3 ret;
		for (size_t row = 0; row < 3; row++)
		{
			for (size_t col = 0; col < 3; col++)
				ret[row][col] = m[col][row];
		}
		return ret;
	}

	bool Matrix3::Inverse(Matrix3& mat, float32 tolerance /*= 1e-06*/) const
	{
		mat[0][0] = m[1][1] * m[2][2] -
			m[1][2] * m[2][1];
		mat[0][1] = m[0][2] * m[2][1] -
			m[0][1] * m[2][2];
		mat[0][2] = m[0][1] * m[1][2] -
			m[0][2] * m[1][1];
		mat[1][0] = m[1][2] * m[2][0] -
			m[1][0] * m[2][2];
		mat[1][1] = m[0][0] * m[2][2] -
			m[0][2] * m[2][0];
		mat[1][2] = m[0][2] * m[1][0] -
			m[0][0] * m[1][2];
		mat[2][0] = m[1][0] * m[2][1] -
			m[1][1] * m[2][0];
		mat[2][1] = m[0][1] * m[2][0] -
			m[0][0] * m[2][1];
		mat[2][2] = m[0][0] * m[1][1] -
			m[0][1] * m[1][0];

		float32 fDet =
			m[0][0] * mat[0][0] +
			m[0][1] * mat[1][0] +
			m[0][2] * mat[2][0];

		if (fabsf(fDet) <= tolerance)
			return false;

		float32 fInvDet = 1.0f / fDet;
		for (size_t i = 0; i < 9; i++)
		{
			mat.val[i] *= fInvDet;
		}

		return true;
	}

	ZFX::Matrix3 Matrix3::Inverse(float32 tolerance /*= 1e-06*/) const
	{
		Matrix3 ret = Matrix3::ZERO;
		Inverse(ret, tolerance);
		return ret;
	}

}