#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <vector>
MYENGINE_SUPPRESS_WARNINGS_END

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

/**
 * @class DirectX12Math.h
 * @brief 数学関数をまとめたクラス
 */

namespace MyMathUtility
{
	// 度からラジアンに変換する定数
	const float PI = 3.141592654f;
	//[degree2Radius * 0~360度]
	constexpr float degree2Radius = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	//ε(イプシロン)
	constexpr float EPSILON = 2.2204460492503131E-16f;

	// 長さを求める(2次元)
	float Vector2Length(const MyMath::Vector2& v);
	// 長さを求める(3次元)
	float Vector3Length(const MyMath::Vector3& v);

	// 正規化する(vector3)
	MyMath::Vector3 MakeNormalize(MyMath::Vector3 v);
	//正規化する(vector2)
	MyMath::Vector2 MakeNormalizeVec2(MyMath::Vector2 v);

	// 単位行列を求める
	MyMath::Matrix4 MakeIdentity();
	// 拡大縮小を求める
	MyMath::Matrix4 MakeScaling(const MyMath::Vector3& scale);
	// 回転行列を求める
	MyMath::Matrix4 MakeRotation(const MyMath::Vector3& rot);
	// 平行移動行列を求める
	MyMath::Matrix4 MakeTranslation(const MyMath::Vector3& trans);

	// 平行投影変換(左手系)
	void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, MyMath::Matrix4& matrix);
	// 平行投影変換(左手系)
	void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, MyMath::Matrix4& matrix);

	// ビュー行列の作成
	MyMath::Matrix4 MakeLookAtLH(MyMath::Vector3& eye, MyMath::Vector3& target, MyMath::Vector3& up);
	// 透視投影作成
	MyMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ);
	// 逆行列
	MyMath::Matrix4 MakeInverse(MyMath::Matrix4& mat);
	//ビューポート行列を求める
	MyMath::Matrix4 MakeViewport(const MyMath::Matrix4& viewport);

	//ワールド→スクリーン座標変換
	MyMath::Vector3 MakeWDivision(MyMath::Vector3& worldPos, MyMath::Matrix4& matrix);

	// 値が等しいか確認する
	bool Approximately(float a, float b);

	// 線形補間(float)
	float Lerp(float a, float b, float t);
	// 最短角度補間
	float LerpShortAngle(const float& a,const float& b,const float& t);
	// 線形補間(Vector3)
	MyMath::Vector3 Lerp(const MyMath::Vector3& a, const MyMath::Vector3& b,const float& t);
	//球面線形補間
	MyMath::Vector3 Slerp(const MyMath::Vector3& a,const MyMath::Vector3& b,const float& t);

	/// <summary>
	/// エルミート曲線
	/// </summary>
	/// <param name="p0">制御点</param>
	/// <param name="p1">制御点</param>
	/// <param name="v0">制御点</param>
	/// <param name="v1">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	/// <returns></returns>
	MyMath::Vector3 HermiteGetPoint(MyMath::Vector3 p0, MyMath::Vector3 p1, MyMath::Vector3 v0, MyMath::Vector3 v1, float t);

	// Sin,Cos,Tan関数(float型)
	float Sin(float sin);
	float Cos(float cos);
	float Tan(float tan);
	void SinCos(float& sin_, float& cos_, float angle);

	float Asin(float sin);
	float Acos(float cos);
	float Atan(float tan);
	float Atan2(float y, float x);

	//max関数(float型)
	float Max(float a, float b);
	//min関数(float型)
	float Min(float a, float b);

	float Dot(MyMath::Vector3 v1,MyMath::Vector3 v2);

	/// <summary>
	/// 距離をflameで割る
	/// </summary>
	/// <param name="x1">スタート地点</param>
	/// <param name="x2">ゴール地点</param>
	/// <param name="flame">フレーム</param>
	void Complement(float& x1, float x2, float flame);

	//vの値を範囲[low, high]に収める。
	float Clamp(float Value, const float low, const float high);
	//vの値を範囲[0 ~ 1]に収める。
	float Clamp0To1(float val);

	/// <summary>
	/// 単振動をした値を返す
	/// </summary>
	/// <param name="time">経過時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="period">周期</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(float time, float amplitude = 10.0f, float period = 60.0f);

	//イージング関数群
	//参照元：https://easings.net/ja 
	float EaseOutCubic(const float startPoint, const float endPoint, float nowFlame, float endFlame);
	float EaseInSine(const float startPoint, const float endPoint, float nowFlame, float endFlame);
	float EaseOutSine(const float startPoint, const float endPoint, float nowFlame, float endFlame);
	float EaseOutQuint(const float startPoint, const float endPoint, float nowFlame, float endFlame);
	float EaseInExpo(const float startPoint, const float endPoint, float nowFlame, float endFlame);
	float EaseInOutBack(const float startPoint, const float endPoint, float nowFlame, float endFlame);

	/// <summary>
	/// ランダム関数
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max"最大値></param>
	/// <returns>ランダムした結果</returns>
	/*int GetRandom(int min,int max);
	bool randFlag;*/

	/// <summary>
	/// オイラー角からクォータニオンへ変換する
	/// </summary>
	/// <param name="rot">オイラー角</param>
	MyMath::Quaternion EulerToQuaternion(MyMath::Vector3 rot);
}

namespace MyMath
{
	// ベクトルと行列の掛け算
	Vector3 Vec3Mat4Mul(const Vector3& vec,const Matrix4& mat);

	// キャットムルーロムスプライン曲線
	Vector3 CatMullRomSpline(std::vector<Vector3>& points, float t);

	/// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegLineOfSeparateAxis(Vector3* sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
}
