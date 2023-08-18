#include "GameCamera.h"

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

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
	camera_->SetEye({ MyMath::GetWorldPosition(cameraTrans) });

	//前方向ベクトルの計算
	MyMath::Vector3 forward(0.0f, 0.0f, 1.0f);
	forward = MyMath::Vec3Mat4Mul(forward, cameraTrans.matWorld);
	camera_->target_ = camera_->eye_ + forward;

	//上方向ベクトルの計算
	MyMath::Vector3 up(0, 1, 0);
	camera_->up_ = MyMath::Vec3Mat4Mul(up, cameraTrans.matWorld);
}

void GameCamera::Reset()
{
}
