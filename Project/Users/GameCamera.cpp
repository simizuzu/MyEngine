#include "GameCamera.h"

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);
	curveData = LevelLoader::LoadFile("curveData");

	camera_->SetEye({ 0.0f,40.0f,0.0f });
	camera_->SetTarget({ 0,0,0 });
	camera_->SetUp({ 1,0,0 });
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
	camera_->SetEye(MyMath::GetWorldPosition(cameraTrans));

	targetTimeRate = timeRate + 0.00025f;
	
	//ベジェ曲線に沿った方向ベクトルの計算
	camera_->SetTarget(MyMathUtility::BezierCurve(curveData->curves, targetTimeRate));
	
	//上方向ベクトルの計算
	MyMath::Vector3 up(0, 1, 0);
	camera_->SetUp(MyMath::Vec3Mat4Mul(up, cameraTrans.matWorld));
	
	//camera_->eye_ = MyMathUtility::BezierCurve(curveData->curves, timeRate);

}

void GameCamera::Reset()
{
}
