#ifndef _ZFX_MATRIX_3_H_
#define _ZFX_MATRIX_3_H_

#include "ZFXPrerequisites.h"

namespace ZFX
{
	class ZFXExport Matrix3
	{
	public:
		union
		{
			float32 val[9];
			float32 m[3][3];
		};

	public:
		inline Matrix3() {}

		inline Matrix3(const float32 m[3][3])
		{
			memcpy(this->m, m, 9 * sizeof(float32));
		}

		inline Matrix3(const Matrix3& mat)
		{
			memcpy(val, mat.val, 9 * sizeof(float32));
		}

		Matrix3(
			float32 f11, float32 f12, float32 f13,
			float32 f21, float32 f22, float32 f23,
			float32 f31, float32 f32, float32 f33)
		{
			m[0][0] = f11;
			m[0][1] = f12;
			m[0][2] = f13;

			m[1][0] = f21;
			m[1][1] = f22;
			m[1][2] = f23;

			m[2][0] = f31;
			m[2][1] = f32;
			m[2][2] = f33;
		}

		inline void Swap(Matrix3& mat)
		{
			for (int i = 0; i < 9; i++)
			{
				std::swap(val[i], mat.val[i]);
			}
		}

		inline float32* operator[](size_t row) const
		{
			assert(row < 3);
			return (float32*)m[row];
		}

		Vector3 GetColumn(size_t col) const;

		void SetColumn(size_t col, const Vector3& v);

		inline Matrix3& operator=(const Matrix3& mat)
		{
			memcpy(val, mat.val, 9 * sizeof(float32));
			return *this;
		}

		bool operator==(const Matrix3& mat) const;

		bool operator!=(const Matrix3& mat) const;

		Matrix3 operator+(const Matrix3& mat) const;

		Matrix3 operator-(const Matrix3& mat) const;

		Matrix3 operator*(const Matrix3& mat) const;

		Matrix3 operator-() const;

		Vector3 operator*(const Vector3& v) const;

		friend Vector3 operator*(const Vector3& v, const Matrix3& mat);

		Matrix3 operator*(float32 scalar) const;

		friend Matrix3 operator*(float32 scalar, const Matrix3& mat);

		Matrix3 Transpose() const;

		bool Inverse(Matrix3& mat, float32 tolerance = 1e-06) const;

		Matrix3 Inverse(float32 tolerance = 1e-06) const;

	public:
		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

	};
}

#endif