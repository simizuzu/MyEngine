#include "GameCamera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file GameCamera.cpp
 * @brief Gameで使うカメラの処理が書かれてあります
 */

void GameCamera::Initialize(Camera* camera, Input* input)
{
	assert(camera);
	assert(input);
	camera_ = camera;
	input_ = input;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);
	curveData = LevelLoader::LoadFile("curveData");

	nowCount = startCount;
	SplinePointLineUp(curveData->curves);
}

void GameCamera::Update()
{
	pos = MyMathUtility::SplinePosition(points, timeRate, startIndex);

	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;
	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1.0f)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex += 1;
			timeRate -= 1.0f;
			startCount = nowCount;
		}
		else
		{
			timeRate = 1.0f;
		}
	}

	target = MyMathUtility::SplinePosition(points, timeRate, startIndex);

	MyMath::Vector3 up(0, 1, 0);
	camera_->SetUp(up);
	/*camera_->SetEye({0,100.0f,0});
	camera_->SetTarget({ 0,0.0f,0.0f });*/

	//pos.y = 100.0f;

	camera_->SetEye(pos);
	camera_->SetTarget(target);
}

void GameCamera::Reset()
{
}

void GameCamera::SplinePointLineUp(std::vector<LevelData::CurveData> curvePoint)
{
	points.resize(curvePoint.size() + 2);
	for (size_t i = 0; i < curvePoint.size(); i++)
	{
		points[i + 1] = curvePoint[i];
		if (i == 0)
		{
			points[i] = curvePoint[i];
		}
		if (i == curvePoint.size() - 1)
		{
			points[i + 2] = curvePoint[i];
		}
	}
}
