#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cmath>
#include <array>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class Matrix3x4.h
 * @brief Matrix3x4のクラス
 */

namespace MyMath
{
	class Matrix3x4
	{
	public:
		std::array<std::array<float,3>,4>m;

		//コンストラクタ
		Matrix3x4();

		Matrix3x4(
			float m00,float m01,float m02,float m03,
			float m10,float m11,float m12,float m13,
			float m20,float m21,float m22,float m23);

		~Matrix3x4() = default;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x4& operator=(const Matrix3x4& _m);

		// 加算
		Matrix3x4& operator+=(const Matrix3x4& mat);
		// 減算
		Matrix3x4& operator-=(const Matrix3x4& mat);
		// 乗算
		Matrix3x4& operator*=(const Matrix3x4& mat);
		// 加算
		Matrix3x4 operator+(const Matrix3x4& mat) const;
		// 減算
		Matrix3x4 operator-(const Matrix3x4& mat) const;
		// 乗算
		Matrix3x4 operator*(const Matrix3x4& mat) const;

		//コピーコンストラクタ
		Matrix3x4(const Matrix3x4&);
	};
}


