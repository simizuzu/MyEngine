#include "GameCamera.h"

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);
	curveData = LevelLoader::LoadFile("curveData");

	//MyMath::Vector3 up(1, 0, 0);
	//camera_->SetUp(up);
	camera_->SetEye({0,0,-10.0f});
	//camera_->SetTarget({0,-1,0});
}

void GameCamera::Update()
{
	//MyMath::Vector3 pos = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	////pos.y = 30;

	////全体の速度を調整できる
	//timeRate += 0.0005f;

	////tが1.0fに到達したとき最初に戻す
	//if (timeRate >= 1.0f)
	//{
	//	timeRate = 0.0f;
	//}

	//MyMath::Vector3 target = MyMathUtility::BezierCurve(curveData->curves, timeRate);

	//MyMath::Vector3 up(0, 1, 0);
	//camera_->SetUp(up);
	//camera_->SetEye(pos);
	//camera_->SetTarget(target);
}

void GameCamera::Reset()
{
}
