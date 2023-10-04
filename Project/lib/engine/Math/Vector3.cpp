#include"Vector3.h"
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cmath>
MYENGINE_SUPPRESS_WARNINGS_END

namespace MyMath
{
	// 零ベクトル
	Vector3::Vector3() :x(0), y(0), z(0) {}

	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	// ベクトルの長さを計算する
	float Vector3::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3& Vector3::normalize()
	{
		float len = length();
		if (len != 0)
		{
			return*this /= len;
		}
		return*this;
	}

	Vector3 Vector3::Norm() const
	{
		Vector3 tmp(*this);

		float len = length();
		if (len != 0)
		{
			return tmp /= len;
		}

		return tmp;
	}

	float Vector3::dot(const Vector3& v)const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3 Vector3::cross(const Vector3& v)const
	{
		Vector3 w;
		w.x = y * v.z - z * v.y;
		w.y = z * v.x - x * v.z;
		w.z = x * v.y - y * v.x;

		return w;
	}

	Vector3 Vector3::copy(const Vector3& v1, const Vector3& v2) const
	{
		Vector3 w1,w2;

		w1 = v1;
		w2 = v2;
		w1 = w2;

		return w1;
	}

	Vector3 Vector3::operator+()const
	{
		return *this;
	}

	Vector3 Vector3::operator-()const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3& Vector3::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3& Vector3::operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	//Vector3 クラスに属さない関数郡
	//二項演算子
	const Vector3 operator+ (const Vector3& v1, const Vector3& v2)
	{
		Vector3 temp(v1);
		return temp += v2;
	}

	const Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		Vector3 temp(v1);
		return temp -= v2;
	}

	const Vector3 operator*(const Vector3& v, float s)
	{
		Vector3 temp(v);
		return temp *= s;
	}

	const Vector3 operator*(float s, const Vector3& v)
	{
		return v * s;
	}

	const Vector3 operator/(const Vector3& v, float s)
	{
		Vector3 temp(v);
		return temp /= s;
	}
}
