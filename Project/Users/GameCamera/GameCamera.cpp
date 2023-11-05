#include "GameCamera.h"
#include "Numbers.h"

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
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * degree90);
	curveData = LevelLoader::LoadFile("curveData");

	nowCount = startCount;
	SplinePointLineUp(curveData->curves);
}

void GameCamera::Update()
{
	pos = MyMathUtility::SplinePosition(points, timeRate, startIndex);

	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / oneSecondFrame;
	timeRate = elapsedTime / maxTime;

	if (timeRate >= static_cast<float>(one))
	{
		if (startIndex < points.size() - three)
		{
			startIndex += static_cast< size_t >(one);
			timeRate -= static_cast< float >(one);
			startCount = nowCount;
		}
		else
		{
			timeRate = static_cast< float >(one);
		}
	}

	target = MyMathUtility::SplinePosition(points, timeRate, startIndex);

	MyMath::Vector3 up(0, 1, 0);
	camera_->SetUp(up);
	camera_->SetEye(pos);
	camera_->SetTarget(target);
}

void GameCamera::Reset()
{
}

void GameCamera::SplinePointLineUp(std::vector<LevelData::CurveData> curvePoint)
{
	points.resize(curvePoint.size() + two);
	for (size_t i = zero; i < curvePoint.size(); i++)
	{
		points[i + one] = curvePoint[i];
		if (i == zero)
		{
			points[i] = curvePoint[i];
		}
		if (i == curvePoint.size() - one)
		{
			points[i + 2] = curvePoint[i];
		}
	}
}
