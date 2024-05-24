#include "SuppressWarning.h"
#include "WorldTransform.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include "Quaternion.h"
#include<cmath>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file Quaternion.cpp
 * @brief Quaternionの処理について書いてあります
 */

namespace MyMath
{
	Quaternion::Quaternion()
	{
		w = 1;
		x = 0;
		y = 0;
		z = 0;
	}

	Quaternion::Quaternion(float x,float y,float z,float w)
	{
		w = w;
		x = x;
		y = y;
		z = z;
	}

	Quaternion Quaternion::Multiply(Quaternion r)
	{
		Quaternion result;

		result.w = w * r.w - x * r.x - y * r.y - z * r.z;
		result.x = y * r.z - z * r.y + r.w * x + w * r.x;
		result.y = z * r.x - x * r.z + r.w * y + w * r.y;
		result.z = x * r.y - y * r.x + r.w * z + w * r.z;

		return result;
	}

	Quaternion Quaternion::IdentityQuaternion()
	{
		Quaternion result;

		result.w = 1;
		result.x = 0;
		result.y = 0;
		result.z = 0;

		return result;
	}

	Quaternion Quaternion::Conjugate()
	{
		Quaternion result;

		result.w = w;
		result.x = -x;
		result.y = -y;
		result.z = -z;

		return result;
	}

	float Quaternion::Norm() const
	{
		float sqrtNorm = sqrt(pow(w,2.0f) + pow(x,2.0f) + pow(y,2.0f) + pow(z,2.0f));
		return sqrtNorm;
	}

	Quaternion Quaternion::Normalize()
	{
		Quaternion result = *this;

		float len = Norm();

		result = result / len;

		return result;
	}

	Quaternion Quaternion::Inverse()
	{
		Quaternion result = Conjugate() / pow(Norm(),2.0f);

		return result;
	}

	Vector3 Quaternion::RotateVector(const Vector3& vector)
	{
		Quaternion result;
		Quaternion VecQua;

		VecQua.w = 0;
		VecQua.x = vector.x;
		VecQua.y = vector.y;
		VecQua.z = vector.z;

		Quaternion conQuaternion = Conjugate();

		result = Multiply(VecQua);
		result = result.Multiply(conQuaternion);

		return { result.x,result.y,result.z };
	}

	Matrix4 Quaternion::MakeRotateMatrix()
	{
		Matrix4 result = MyMathUtility::MakeIdentity();

		result.m[ 0 ][ 0 ] = w * w + x * x - y * y - z * z;
		result.m[ 0 ][ 1 ] = 2 * ( x * y + w * z );
		result.m[ 0 ][ 2 ] = 2 * ( x * z - w * y );
		result.m[ 1 ][ 0 ] = 2 * ( x * y - w * z );
		result.m[ 1 ][ 1 ] = w * w - x * x + y * y - z * z;
		result.m[ 1 ][ 2 ] = 2 * ( y * z + w * x );
		result.m[ 2 ][ 0 ] = 2 * ( x * z + w * y );
		result.m[ 2 ][ 1 ] = 2 * ( y * z - w * x );
		result.m[ 2 ][ 2 ] = w * w - x * x - y * y + z * z;

		return result;
	}

	Quaternion Quaternion::MakeAxisAngle(const MyMath::Vector3& axsi,float angle)
	{
		Quaternion result;

		result.w = cos(angle / 2);

		Vector3 VecResult = axsi * sin(angle / 2);

		result.x = VecResult.x;
		result.y = VecResult.y;
		result.z = VecResult.z;

		return result;
	}

	MyMath::Vector3 Quaternion::QuaternionToEuler()
	{
		MyMath::Vector3 result;

		float ww = w * w;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		float xx = x * x;
		float xy = x * y;
		float xz = x * z;

		float yy = y * y;
		float yz = y * z;

		float zz = z * z;

		result.x = atan2(twice * ( yz + wx ),ww - xx - yy + zz);
		result.y = asin(twice * ( wy - xz ));
		result.z = atan2(twice * ( xy + wz ),ww + xx - yy - zz);

		return result;
	}

	Quaternion Quaternion::DirectionToDirection(const MyMath::Vector3& u,const MyMath::Vector3& v)
	{
		// uとvを正規化して内積を求める。u,vを単位ベクトル前提とするなら正規化は不要
		float dot = u.x * v.x + u.y * v.y + u.z * v.z;

		// 外積をとる
		Vector3 cross = u.cross(v);

		// 軸は単位ベクトルである必要があるので正規化
		Vector3 axis = cross.normalize();

		// 単位ベクトルでない席を取っているのでacosで角度を求める
		float theta = acosf(dot);

		// axisとthetaで任意軸回転を作って返す
		Quaternion result;

		result = MakeAxisAngle(axis,theta);

		return result;
	}

	Quaternion Quaternion::Slerp(const Quaternion& p,float t)
	{
		float cos = Dot(p);

		Quaternion t2 = *this;

		if ( cos < 0.0f )
		{
			cos = -cos;
			t2 = -*this;
		}

		if ( cos >= 1.0f - 0.0005f )
		{
			return ( 1.0f - t ) * p + t * t2;
		}

		float k0 = t;
		float k1 = 1.0f - t;

		float  theta = acosf(cos);

		k0 = ( sinf(theta * k0) / sinf(theta) );
		k1 = ( sinf(theta * k1) / sinf(theta) );

		return  k0 * p + k1 * t2;
	}

	float Quaternion::Dot(const Quaternion& q) const
	{
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}
	Quaternion Quaternion::operator+()
	{
		return *this;
	}

	Quaternion Quaternion::operator-()const
	{
		return Quaternion(-x,-y,-z,-w);
	}

	Quaternion& Quaternion::operator=(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q)
	{

		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	Quaternion& Quaternion::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
	}

	Quaternion& Quaternion::operator/=(float s)
	{
		return *this *= 1.0f / s;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		float rx,ry,rz,rw;

		rx = w * q.x + q.w * x + y * q.z + -z * q.y;
		ry = w * q.y + q.w * y + z * q.x + -x * q.z;
		rz = w * q.z + q.w * z + x * q.y + -y * q.x;
		rw = w * q.w + -x * q.x + -y * q.y + -z * q.z;

		x = rx;
		y = ry;
		z = rz;
		w = rw;

		return *this;
	}

	Quaternion::Quaternion(const Quaternion&)
	{
	}

	const Quaternion operator+(const Quaternion& q1,const Quaternion& q2)
	{
		Quaternion temp = q1;

		return temp += q2;
	}

	const Quaternion operator-(const Quaternion& q1,const Quaternion& q2)
	{
		Quaternion result = q1;
		return result -= q2;
	}

	const Quaternion operator*(const Quaternion& q1,const Quaternion& q2)
	{
		Quaternion temp = q1;

		return temp *= q2;
	}

	const Quaternion operator*(const Quaternion& q,float s)
	{
		Quaternion temp = q;

		return temp *= s;
	}

	const Quaternion operator*(float s,const Quaternion& q)
	{
		Quaternion temp = q;

		return temp *= s;
	}

	const Quaternion operator/(const Quaternion& q,float s)
	{
		Quaternion result = q;
		result /= s;
		return result;
	}

	const Vector3 operator*(Quaternion q,Vector3 vec)
	{
		float x = q.x * 2.0f;
		float y = q.y * 2.0f;
		float z = q.z * 2.0f;
		float xx = q.x * x;
		float yy = q.y * y;
		float zz = q.z * z;
		float xy = q.x * y;
		float xz = q.x * z;
		float yz = q.y * z;
		float wx = q.w * x;
		float wy = q.w * y;
		float wz = q.w * z;

		Vector3 temp;

		temp.x = ( 1.0f - ( yy + zz ) ) * vec.x + ( xy - wz ) * vec.y + ( xz + wy ) * vec.z;
		temp.y = ( xy + wz ) * vec.x + ( 1.0f - ( xx + zz ) ) * vec.y + ( yz - wx ) * vec.z;
		temp.z = ( xz - wy ) * vec.x + ( yz + wx ) * vec.y + ( 1.0f - ( xx + yy ) ) * vec.z;

		return temp;
	}
}
