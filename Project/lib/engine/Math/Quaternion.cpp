#include "Quaternion.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
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
		w_ = 1;
		x_ = 0;
		y_ = 0;
		z_ = 0;
	}

	Quaternion::Quaternion(float x,float y,float z,float w)
	{
		w_ = w;
		x_ = x;
		y_ = y;
		z_ = z;
	}

	Quaternion Quaternion::Multiply(Quaternion r)
	{
		Quaternion result;

		result.w_ = w_ * r.w_ - x_ * r.x_ - y_ * r.y_ - z_ * r.z_;
		result.x_ = y_ * r.z_ - z_ * r.y_ + r.w_ * x_ + w_ * r.x_;
		result.y_ = z_ * r.x_ - x_ * r.z_ + r.w_ * y_ + w_ * r.y_;
		result.z_ = x_ * r.y_ - y_ * r.x_ + r.w_ * z_ + w_ * r.z_;

		return result;
	}

	Quaternion Quaternion::IdentityQuaternion()
	{
		Quaternion result;

		result.w_ = 1;
		result.x_ = 0;
		result.y_ = 0;
		result.z_ = 0;

		return result;
	}

	Quaternion Quaternion::Conjugate()
	{
		Quaternion result;

		result.w_ = w_;
		result.x_ = -x_;
		result.y_ = -y_;
		result.z_ = -z_;

		return result;
	}

	float Quaternion::Norm() const
	{
		float sqrtNorm = sqrt(pow(w_,2.0f) + pow(x_,2.0f) + pow(y_,2.0f) + pow(z_,2.0f));
		return sqrtNorm;
	}

	Quaternion Quaternion::Normalize()
	{
		Quaternion result = *this;

		float len = Norm();

		result = result / len;

		return result;
	}

	Quaternion Quaternion::INverse()
	{
		Quaternion result = Conjugate() / pow(Norm(),2.0f);

		return result;
	}

	Vector3 Quaternion::RotateVector(const Vector3& vector)
	{
		Quaternion result;
		Quaternion VecQua;

		VecQua.w_ = 0;
		VecQua.x_ = vector.x;
		VecQua.y_ = vector.y;
		VecQua.z_ = vector.z;

		Quaternion conQuaternion = Conjugate();

		result = Multiply(VecQua);
		result = result.Multiply(conQuaternion);

		return { result.x_,result.y_,result.z_ };
	}

	Matrix4 Quaternion::MakeRotateMatrix()
	{
		Matrix4 result = MyMathUtility::MakeIdentity();

		result.m[ 0 ][ 0 ] = w_ * w_ + x_ * x_ - y_ * y_ - z_ * z_;
		result.m[ 0 ][ 1 ] = 2 * ( x_ * y_ + w_ * z_ );
		result.m[ 0 ][ 2 ] = 2 * ( x_ * z_ - w_ * y_ );
		result.m[ 1 ][ 0 ] = 2 * ( x_ * y_ - w_ * z_ );
		result.m[ 1 ][ 1 ] = w_ * w_ - x_ * x_ + y_ * y_ - z_ * z_;
		result.m[ 1 ][ 2 ] = 2 * ( y_ * z_ + w_ * x_ );
		result.m[ 2 ][ 0 ] = 2 * ( x_ * z_ + w_ * y_ );
		result.m[ 2 ][ 1 ] = 2 * ( y_ * z_ - w_ * x_ );
		result.m[ 2 ][ 2 ] = w_ * w_ - x_ * x_ - y_ * y_ + z_ * z_;

		return result;
	}

	Quaternion Quaternion::MakeAxisAngle(const MyMath::Vector3& axsi,float angle)
	{
		Quaternion result;

		result.w_ = cos(angle / 2);

		Vector3 VecResult = axsi * sin(angle / 2);

		result.x_ = VecResult.x;
		result.y_ = VecResult.y;
		result.z_ = VecResult.z;

		return result;
	}

	MyMath::Vector3 Quaternion::QuaternionToEulerAngles()
	{
		MyMath::Vector3 result;

		float q0

		result.x = atan2(2.0f * ( q2q3 + q0q1 ),q0q0 - q1q1 - q2q2 + q3q3);
		result.y = asin(2.0f * ( q0q2 - q1q3 ));
		result.z = atan2(2.0f * ( q1q2 + q0q3 ),q0q0 + q1q1 - q2q2 - q3q3);

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
		return x_ * q.x_ + y_ * q.y_ + z_ * q.z_ + w_ * q.w_;
	}
	Quaternion Quaternion::operator+()
	{
		return *this;
	}

	Quaternion Quaternion::operator-()const
	{
		return Quaternion(-x_,-y_,-z_,-w_);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		x_ += q.x_;
		y_ += q.y_;
		z_ += q.z_;
		w_ += q.w_;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q)
	{

		x_ -= q.x_;
		y_ -= q.y_;
		z_ -= q.z_;
		w_ -= q.w_;

		return *this;
	}

	Quaternion& Quaternion::operator*=(float s)
	{
		x_ *= s;
		y_ *= s;
		z_ *= s;
		w_ *= s;

		return *this;
	}

	Quaternion& Quaternion::operator/=(float s)
	{
		return *this *= 1.0f / s;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		float rx,ry,rz,rw;

		rx = w_ * q.x_ + q.w_ * x_ + y_ * q.z_ + -z_ * q.y_;
		ry = w_ * q.y_ + q.w_ * y_ + z_ * q.x_ + -x_ * q.z_;
		rz = w_ * q.z_ + q.w_ * z_ + x_ * q.y_ + -y_ * q.x_;
		rw = w_ * q.w_ + -x_ * q.x_ + -y_ * q.y_ + -z_ * q.z_;

		x_ = rx;
		y_ = ry;
		z_ = rz;
		w_ = rw;

		return *this;
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
		float x = q.x_ * 2.0f;
		float y = q.y_ * 2.0f;
		float z = q.z_ * 2.0f;
		float xx = q.x_ * x;
		float yy = q.y_ * y;
		float zz = q.z_ * z;
		float xy = q.x_ * y;
		float xz = q.x_ * z;
		float yz = q.y_ * z;
		float wx = q.w_ * x;
		float wy = q.w_ * y;
		float wz = q.w_ * z;

		Vector3 temp;

		temp.x = ( 1.0f - ( yy + zz ) ) * vec.x + ( xy - wz ) * vec.y + ( xz + wy ) * vec.z;
		temp.y = ( xy + wz ) * vec.x + ( 1.0f - ( xx + zz ) ) * vec.y + ( yz - wx ) * vec.z;
		temp.z = ( xz - wy ) * vec.x + ( yz + wx ) * vec.y + ( 1.0f - ( xx + yy ) ) * vec.z;

		return temp;
	}
}
