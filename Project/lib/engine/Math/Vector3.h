#pragma once

namespace MyMath
{
	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	class Vector3
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分

	public:
		//コンストラクト
		Vector3();                //ゼロベクトルとして生成
		Vector3(float x, float y, float z);//x成分,y成分を指定しての生成
		
		//　メンバ関数
		float length() const;              //ノルム(長さ)を求める
		Vector3& normalize();              //正規化する
		Vector3 Norm()const;
		float dot(const Vector3& v)const;  //内積を求める
		Vector3 cross(const Vector3& v)const; //外積を求める
		Vector3 copy(const Vector3& v1, const Vector3& v2)const;

		//単項演算子オーバーロード
		Vector3 operator+() const;
		Vector3 operator-() const;

		//代入演算子オーバーロード
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	//2項演算子オ－バーロード
	//※いろんな引数(引数の型と順序)のパターンに対応するために、以下のように準備している
	const Vector3 operator+(const Vector3& v1, const Vector3& v2);
	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);
}
