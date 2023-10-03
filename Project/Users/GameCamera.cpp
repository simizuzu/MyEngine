#include "GameCamera.h"
#include <imgui.h>

void GameCamera::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * 90.0f);
	curveData = LevelLoader::LoadFile("curveTest");

	//MyMath::Vector3 up(1, 0, 0);
	//camera_->SetUp(up);
	camera_->SetEye({0,0,-10.0f});
	//camera_->SetTarget({0,-1,0});

	nowCount = startCount; 
	points.resize(curveData->curves.size() +2);
	for (size_t i = 0; i < curveData->curves.size(); i++)
	{
		points[i+1] = curveData->curves[i];
		if (i==0)
		{
			points[i] = curveData->curves[i];
		}
		if (i == curveData->curves.size() - 1)
		{
			points[i+2] = curveData->curves[i];
		}
	}
}

void GameCamera::Update()
{
	ImGui::Begin("Points");
	ImGui::Text("Points(%d)", curveData->curves.size());
	ImGui::End();

	pos = MyMathUtility::SplinePosition(points, timeRate,startIndex);
	//pos.y = 10;

	//全体の速度を調整できる
	//timeRate += 0.005f;

	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;
	timeRate = elapsedTime / maxTime;


	if (timeRate >= 1.0f)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex += 1.0f;
			timeRate -= 1.0f;
			startCount = nowCount;
		}
		else
		{
			timeRate = 1.0f;
		}
	}

	target = MyMathUtility::SplinePosition(points, timeRate,startIndex);
	
	MyMath::Vector3 up(0, 1, 0);
	camera_->SetUp(up);
	camera_->SetEye(pos);
	camera_->SetTarget(target);
}

void GameCamera::Reset()
{
}
