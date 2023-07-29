﻿#pragma once

namespace MyMath
{
	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	class Vector4
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分
		float w;//w

	public:
		//コンストラクタ
		Vector4();//零ベクトル
		Vector4(float x, float y, float z, float w);//成分を指定して生成

		float length()const;//ノルムを求める
		float lengthSquared()const;
		Vector4 normalization()const;//正規化する
		Vector4& normal();//正規化する
		float dot(const Vector4& v)const;//内積を求める
		Vector4 cross(const Vector4& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector4 operator+() const;
		Vector4 operator-() const;

		// 代入演算子オーバーロード
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);

	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);
}
