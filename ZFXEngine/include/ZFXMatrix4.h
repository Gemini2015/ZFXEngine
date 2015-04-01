#ifndef _ZFX_MATRIX_4_H_
#define _ZFX_MATRIX_4_H_

#include "ZFXPrerequisites.h"
#include "ZFXVector4.h"
#include "ZFXPlane.h"
#include "ZFXMatrix3.h"

namespace ZFX
{
	class Matrix4
	{
	public:
		union
		{
			float32 val[16];
			float32 m[4][4];
		};

	public:
		inline Matrix4() {}

		inline Matrix4(
			float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44
			)
		{
			m[0][0] = f11;
			m[0][1] = f12;
			m[0][2] = f13;
			m[0][3] = f14;

			m[1][0] = f21;
			m[1][1] = f22;
			m[1][2] = f23;
			m[1][3] = f24;

			m[2][0] = f31;
			m[2][1] = f32;
			m[2][2] = f33;
			m[2][3] = f34;

			m[3][0] = f41;
			m[3][1] = f42;
			m[3][2] = f43;
			m[3][3] = f44;
		}

		inline Matrix4(const Matrix3& mat)
		{
			operator=(IDENTITY);
			operator=(mat);
		}

		/*inline Matrix4(const Quaternion& rot)
		{
		Matrix3 m3x3;
		rot.ToRotationMatrix(m3x3);
		operator=(IDENTITY);
		operator=(m3x3);
		}*/

		inline void Identity()
		{
			for (size_t i = 0; i < 16; i++)
				val[i] = 0.0f;
			m[0][0] = 1.0f;
			m[1][1] = 1.0f;
			m[2][2] = 1.0f;
			m[3][3] = 1.0f;
		}

		inline void Swap(Matrix4& mat)
		{
			for (size_t i = 0; i < 16; i++)
				std::swap(val[i], mat.val[i]);
		}

		inline float32* operator[](size_t row)
		{
			assert(row < 4);
			return m[row];
		}

		inline const float32* operator[](size_t row) const
		{
			assert(row < 4);
			return m[row];
		}

		inline Matrix4 operator*(const Matrix4& mat) const
		{
			Matrix4 ret;
			ret.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
			ret.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
			ret.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
			ret.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

			ret.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
			ret.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
			ret.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
			ret.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

			ret.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
			ret.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
			ret.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
			ret.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

			ret.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
			ret.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
			ret.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
			ret.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

			return ret;
		}

		inline Vector4 operator * (const Vector4& v) const
		{
			return Vector4(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
				);
		}

		inline Plane operator * (const Plane& p) const
		{
			Plane ret;
			Matrix4 invTrans = Inverse().Transpose();
			Vector4 v4(p.normal.x, p.normal.y, p.normal.z, p.d);
			v4 = invTrans * v4;
			ret.normal.x = v4.x;
			ret.normal.y = v4.y;
			ret.normal.z = v4.z;
			ret.d = (v4.w) / ret.normal.GetSqrLength();

			return ret;
		}

		inline Matrix4 operator*(float32 scalar) const
		{
			Matrix4 ret;
			for (size_t i = 0; i < 16; i++)
			{
				ret.val[i] = val[i] * scalar;
			}
			return ret;
		}

		inline Matrix4 operator+(const Matrix4& mat) const
		{
			Matrix4 ret;
			for (size_t i = 0; i < 16; i++)
			{
				ret.val[i] = val[i] + mat.val[i];
			}
			return ret;
		}

		inline Matrix4 operator-(const Matrix4& mat) const
		{
			Matrix4 ret;
			for (size_t i = 0; i < 16; i++)
			{
				ret.val[i] = val[i] - mat.val[i];
			}
			return ret;
		}

		inline bool operator==(const Matrix4& mat) const
		{
			for (size_t i = 0; i < 16; i++)
			{
				if (val[i] != mat.val[i])
					return false;
			}
			return true;
		}

		inline bool operator!=(const Matrix4& mat) const
		{
			for (size_t i = 0; i < 16; i++)
			{
				if (val[i] == mat.val[i])
					return false;
			}
			return true;
		}

		inline void operator=(const Matrix3& mat)
		{
			m[0][0] = mat.m[0][0]; m[0][1] = mat.m[0][1]; m[0][2] = mat.m[0][2];
			m[1][0] = mat.m[1][0]; m[1][1] = mat.m[1][1]; m[1][2] = mat.m[1][2];
			m[2][0] = mat.m[2][0]; m[2][1] = mat.m[2][1]; m[2][2] = mat.m[2][2];
		}

		inline Matrix4 Transpose(void) const
		{
			return Matrix4(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		inline Matrix4 GetColumnMajorOrderMatrix() const
		{
			return this->Transpose();
		}

		/************************************************************************/
		/*
			Translate
			*/
		/************************************************************************/
		inline void SetTranslate(const Vector3& v)
		{
			m[0][3] = v.x;
			m[1][3] = v.y;
			m[2][3] = v.z;
		}

		inline void SetTranslate(float32 x, float32 y, float32 z)
		{
			m[0][3] = x;
			m[1][3] = y;
			m[2][3] = z;
		}

		inline Vector3 GetTranslate() const
		{
			return Vector3(m[0][3], m[1][3], m[2][3]);
		}

		inline void MakeTranslate(const Vector3& v)
		{
			Identity();
			SetTranslate(v);
		}

		inline void MakeTranslate(float32 x, float32 y, float32 z)
		{
			Identity();
			SetTranslate(x, y, z);
		}

		inline static Matrix4 GetTranslate(const Vector3& v)
		{
			Matrix4 ret = IDENTITY;
			ret.m[0][3] = v.x;
			ret.m[1][3] = v.y;
			ret.m[2][3] = v.z;
			return ret;
		}

		inline static Matrix4 GetTranslate(float32 x, float32 y, float32 z)
		{
			Matrix4 ret = IDENTITY;
			ret.m[0][3] = x;
			ret.m[1][3] = y;
			ret.m[2][3] = z;
			return ret;
		}

		/************************************************************************/
		/*
			Scale
			*/
		/************************************************************************/
		inline void SetScale(const Vector3& v)
		{
			m[0][0] = v.x;
			m[1][1] = v.y;
			m[2][2] = v.z;
		}

		inline void SetScale(float32 x, float32 y, float32 z)
		{
			m[0][0] = x;
			m[1][1] = y;
			m[2][2] = z;
		}

		inline Vector3 GetScale() const
		{
			return Vector3(m[0][0], m[1][1], m[2][2]);
		}

		inline void MakeScale(const Vector3& v)
		{
			Identity();
			SetScale(v);
		}

		inline void MakeScale(float32 x, float32 y, float32 z)
		{
			Identity();
			SetScale(x, y, z);
		}

		inline static Matrix4 GetScale(const Vector3& v)
		{
			Matrix4 ret = IDENTITY;
			ret.m[0][0] = v.x;
			ret.m[1][1] = v.y;
			ret.m[2][2] = v.z;
			return ret;
		}

		inline static Matrix4 GetScale(float32 x, float32 y, float32 z)
		{
			Matrix4 ret = IDENTITY;
			ret.m[0][0] = x;
			ret.m[1][1] = y;
			ret.m[2][2] = z;
			return ret;
		}

		inline Matrix3 GetMatrix3() const
		{
			Matrix3 ret;
			ret.m[0][0] = m[0][0];
			ret.m[0][1] = m[0][1];
			ret.m[0][2] = m[0][2];
			ret.m[1][0] = m[1][0];
			ret.m[1][1] = m[1][1];
			ret.m[1][2] = m[1][2];
			ret.m[2][0] = m[2][0];
			ret.m[2][1] = m[2][1];
			ret.m[2][2] = m[2][2];
			return ret;
		}

		Matrix4 Inverse() const;

		inline bool IsAffine() const
		{
			return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
		}

		Matrix4 InverseAffine() const;

	public:
		static const Matrix4 ZERO;
		static const Matrix4 ZEROAFFINE;
		static const Matrix4 IDENTITY;
	};
}

#endif