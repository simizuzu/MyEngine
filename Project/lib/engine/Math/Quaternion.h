#pragma once
#include"DirectX12Math.h"
#include "WorldTransform.h"

/**
 * @class Quaternion.h
 * @brief Quaternionのクラス
 */

namespace MyMath
{
	class Quaternion
	{
	public:
		float x_;
		float y_;
		float z_;
		float w_;

		//コンストラクタ
		Quaternion();
		Quaternion(float x, float y, float z, float w);

		//Quaternionの積
		Quaternion Multiply(Quaternion r);
		//単位Quaternionを返す
		Quaternion IdentityQuaternion();
		//共役Quaternionを返す
		Quaternion Conjugate();
		//Quaternionのnormを返す
		float Norm() const;
		//正規化したQuaternionを返す
		Quaternion Normalize();
		//逆Quaternionを返す
		Quaternion INverse();
		//ベクトルをQuaternionで回転させた結果のベクトルを求める
		MyMath::Vector3 RotateVector(const MyMath::Vector3& vector);
		//Quaternionから回転行列を求める
		MyMath::Matrix4 MakeRotateMatrix();
		//任意軸回転を表すQuaternionの生成
		Quaternion MakeAxisAngle(const MyMath::Vector3& axsi, float angle);
		//クォータニオンからオイラー角へ変換
		MyMath::Vector3 QuaternionToEulerAngles(float q0,float q1,float q2,float q3);
		
		/// <summary>
		/// 特定の方向に向かせる
		/// </summary>
		/// <param name="u">無回転のときの向き</param>
		/// <param name="v">向かせたい特定の方向</param>
		/// <returns>回転軸[w = u*v]</returns>
		Quaternion DirectionToDirection(const MyMath::Vector3& u, const MyMath::Vector3& v);

		//球面線形補間
		Quaternion Slerp(const Quaternion& p, float t);
		//内積
		float Dot(const Quaternion& q)const;


		//単項演算子オーバーロード
		Quaternion operator + ();
		Quaternion operator - ()const;

		//代入演算子オーバーロード
		Quaternion& operator += (const Quaternion& q);
		Quaternion& operator -= (const Quaternion& q);
		Quaternion& operator *= (float s);
		Quaternion& operator /= (float s);
		Quaternion& operator *= (const Quaternion& q);
	};

	//2項演算子オーバーロード
	const Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator * (const Quaternion& q, float s);
	const Quaternion operator * (float s, const Quaternion& q);
	const Quaternion operator / (const Quaternion& q, float s);
	const MyMath::Vector3 operator*(Quaternion q, MyMath::Vector3 vec);
}
