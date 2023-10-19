#include "FPSCamera.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file FPSCamera.cpp
 * @brief FPSCameraの処理
 */

	void FPSCamera::Initialize(Camera* camera)
{
	assert(camera);

	camera_.reset(camera);
}

void FPSCamera::Update()
{
	MyMath::Vector3 eye,target;

}

void FPSCamera::Move()
{


}

void FPSCamera::ConvertAngleToThreedimension(MyMath::Vector2 value)
{
	//視点・注視点 → 回転座標への変換
	HAngle = GetHAngle(camera_->GetEye(),camera_->GetTarget());
	VAngle = GetVAngle(camera_->GetEye(),camera_->GetTarget());

	HAngle -= value.x; //水平方向への回転
	VAngle += value.y; //素直方向への回転

	//回転座標 → 3次元座標への変換
	//視点座標の取得
	MyMath::Vector3 vEye;
	vEye = camera_->GetEye();

	//注視点座標の初期化(後で代入する)
	MyMath::Vector3 vTarget(0.0f,0.0f,0.0f);

	//HAngle,VAngleを行列に変換する


	//視点座標を取得する
	MyMath::Vector3 eye,target;

}

float FPSCamera::GetHAngle(const MyMath::Vector3& eye,const MyMath::Vector3& target)
{
	//注視点の向きベクトルを求める
	MyMath::Vector3 dir = target - eye;
	dir.normalize();

	//HAngle(XZ平面での角度)を求める
	float deg = MyMathUtility::Atan2(-dir.z,dir.x);
	deg *= MyMathUtility::degree2Radius;

	//調整角度(Z方向を基準に回転する)
	float adjustmentAngle = 90.0f;
	deg += adjustmentAngle;

	// -180～180に丸める
	if ( deg > 180.0f )
	{
		deg -= 360.0f;
	}
	if ( deg < -180.0f )
	{
		deg += 360.0f;
	}

	return deg;
}

float FPSCamera::GetVAngle(const MyMath::Vector3& eye,const MyMath::Vector3& target)
{
	//注視点の向きベクトルを求める
	MyMath::Vector3 dir = target - eye;
	dir = dir.normalize();

	float fFront;
	{
		MyMath::Vector3 vFront;
		vFront.copy(vFront,dir);
		vFront.y = 0;
		fFront = vFront.length();
	}

	// Y軸とXZ平面の前方方向との角度を求める
	float deg = MyMathUtility::Atan2(-dir.z,dir.x);
	deg *= MyMathUtility::degree2Radius;

	// 可動範囲は -90～90
	if ( deg > 90.0f )
	{
		deg = 180.0f - deg;
	}
	if ( deg < -90.0f )
	{
		deg = -180.0f - deg;
	}

	return deg;
}
