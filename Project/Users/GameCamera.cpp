#include "GameCamera.h"

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);

	camera_->SetEye({ 0,100.0f,0 });
	camera_->SetTarget({ 0,0,0 });
	

	curveData = LevelLoader::LoadFile("curveData");
}

void GameCamera::Update()
{
	////経過時間(elapsedTime[s])の計算
	//nowCount++;
	//elapsedCount = nowCount - startCount;
	//float elapsedTime = static_cast<float> (elapsedCount) / 60.f;
	////経過時間(elapsedTime)/全体の時間(maxTime)を0~1(timeRate)にする
	//timeRate = elapsedTime / maxTime;

	//targetTimeRate = timeRate + 0.001f;

	////カメラの座標にベジェ曲線の値を入れる
	//cameraTrans.translation_ = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	//cameraTrans.MakeMatWorld();
	//camera_->eye_ = MyMath::GetWorldPosition(cameraTrans);
	//camera_->eye_ = { 0.0f,100.0f,0.0f };

	////ベジェ曲線に沿った方向ベクトルの計算
	//camera_->target_ = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	//
	////上方向ベクトルの計算
	//MyMath::Vector3 up(0, 1, 0);
	//camera_->up_ = MyMath::Vec3Mat4Mul(up, cameraTrans.matWorld);
}

void GameCamera::Reset()
{
}
