#pragma once

#include <cmath>
#include <array>
#include "Vector3.h"

namespace MyMath
{
	class Matrix4
	{
	public:
		std::array<std::array<float, 4>, 4>m;

		// コンストラクタ
		Matrix4();

		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4& operator=(const Matrix4& _m);

		// 加算
		Matrix4& operator+=(const Matrix4& mat);
		// 減算
		Matrix4& operator-=(const Matrix4& mat);
		// 乗算
		Matrix4& operator*=(const Matrix4& mat);
		// 加算
		Matrix4 operator+(const Matrix4& mat) const;
		// 減算
		Matrix4 operator-(const Matrix4& mat) const;
		// 乗算
		Matrix4 operator*(const Matrix4& mat) const;
	};
}


