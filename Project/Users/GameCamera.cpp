#include "GameCamera.h"

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);

	curveData = LevelLoader::LoadFile("curveData");
}

void GameCamera::Update()
{
	//経過時間(elapsedTime[s])の計算
	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.f;
	//経過時間(elapsedTime)/全体の時間(maxTime)を0~1(timeRate)にする
	timeRate = elapsedTime / maxTime;

	//カメラの座標にベジェ曲線の値を入れる
	cameraTrans.translation_ = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	cameraTrans.MakeMatWorld();
	camera_->eye_ = MyMath::GetWorldPosition(cameraTrans);

	//ベジェ曲線に沿った方向ベクトルの計算
	targetTimeRate = timeRate + 0.002f;
	camera_->target_ = MyMathUtility::BezierCurve(curveData->curves, targetTimeRate);
	
	//上方向ベクトルの計算
	MyMath::Vector3 up(0, 1, 0);
	camera_->up_ = MyMath::Vec3Mat4Mul(up, cameraTrans.matWorld);

}

void GameCamera::Reset()
{
}
