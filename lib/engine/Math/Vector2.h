﻿#pragma once

namespace MyMath
{
	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	class Vector2
	{
	public:
		float x;//x成分
		float y;//y成分

	public:
		// コンストラクタ
		Vector2();//ゼロベクトルとして生成
		Vector2(float x, float y);//x成分、y成分を指定しての生成

		//メンバ関数
		float length()const;//ノルムを求める
		Vector2& normalized();//正規化する
		float dot(const Vector2& v)const;//内積を求める
		float cross(const Vector2& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector2 operator+()const;
		Vector2 operator-()const;

		//代入演算子オーバーロード
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	//2項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);
}
