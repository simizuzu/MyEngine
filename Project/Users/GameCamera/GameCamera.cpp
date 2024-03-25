#include "GameCamera.h"
#include "Numbers.h"
#include "Quaternion.h"
#include "WorldTransform.h"

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

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
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

	camera_->SetTranslation(pos);
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

void GameCamera::OnCollision()
{
}

MyMath::Vector3 GameCamera::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(camera_->GetMatWorld());
	return worldPos;
}
