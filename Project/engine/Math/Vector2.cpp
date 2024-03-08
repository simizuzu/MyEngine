#include"Vector2.h"
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include<cmath>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file Vector2.cpp
 * @brief Vector2の処理について書いてあります
 */

namespace MyMath
{
	// 零ベクトル
	Vector2::Vector2() : x(0.0f), y(0.0f) {}

	Vector2::Vector2(float x, float y) : x(x), y(y) {}

	// ベクトルの長さを計算する
	float Vector2::length() const
	{
		return std::sqrt(x * x + y * y);
	}

	// 正規化ベクトル
	Vector2& Vector2::normalized()
	{
		float len = length();
		if (len != 0)
		{
			return *this /= len;

		}

		return *this;
	}

	// もう一方のベクトルとの内積
	float Vector2::dot(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}
	//外積を求める
	float Vector2::cross(const Vector2& v) const
	{
		return x * v.y - v.x * y;
	}

	//単項演算子オーバーロード
	Vector2 Vector2::operator+() const
	{
		return *this;
	}

	Vector2 Vector2::operator-() const
	{
		return  Vector2(-x, -y);
	}

	//代入演算子オーバーロード
	Vector2& Vector2::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vector2& Vector2::operator*=(float s)
	{
		x *= s;
		y *= s;

		return *this;
	}

	Vector2& Vector2::operator/=(float s)
	{
		x /= s;
		y /= s;

		return *this;
	}

	//Vector2 クラスに属さない関数群
	//二項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2)
	{
		Vector2 temp(v1);
		return temp += v2;
	}

	const Vector2 operator-(const Vector2& v1, const Vector2& v2)
	{
		Vector2 temp(v1);
		return temp -= v2;
	}

	const Vector2 operator*(const Vector2& v, float s)
	{
		Vector2 temp(v);
		return temp *= s;
	}

	const Vector2 operator*(float s, const Vector2& v)
	{
		return v * s;
	}

	const Vector2 operator/(const Vector2& v, float s)
	{
		Vector2 temp(v);
		return temp /= s;
	}
}
