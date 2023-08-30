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
	MyMath::Vector3 pos = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	pos.y = 30 ;
	//MyMath::Vector3 pos = { 0,30,0 };;

	timeRate += 0.001f;
	
	if (timeRate >= 1.0f)
	{
		timeRate = 0.0f;
	}

	//MyMath::Vector3 target = { 0,0,0 };
	MyMath::Vector3 target = MyMathUtility::BezierCurve(curveData->curves, timeRate);
	
	//上方向ベクトルの計算
	//MyMath::Vector3 up(0, 1, 0);
	MyMath::Vector3 up(1, 0, 0);
	camera_->SetUp(up);
	camera_->SetEye(pos);
	camera_->SetTarget(target);
}

void GameCamera::Reset()
{
}
