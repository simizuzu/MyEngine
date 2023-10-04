#include "TitleAnimation.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

void TitleAnimation::Initalize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	demoCurve_ = LevelLoader::LoadFile("demoEnemyTest");;

#pragma region Sprite
	texBack = TextureManager::Load("Resources/Texture//Scene/title.png");
	texClickStart = TextureManager::Load("Resources/Texture/Scene/clickStart.png");

	spriteBack_ = std::make_unique<Sprite>();
	spriteClickStart_ = std::make_unique<Sprite>();

	spriteBack_->Initialize();
	spriteClickStart_->Initialize();
#pragma endregion

#pragma region Obj
	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	boxModel_.reset(ObjModel::LoadFromObj("box"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	boxObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	groundObj_->SetModel(groundModel_.get());
	boxObj_->SetModel(groundModel_.get());

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	boxTrans.Initialize();

	float scale = 900.0f;
	skydomeTrans.SetScale({ scale,scale,scale });
#pragma endregion
}

void TitleAnimation::Update()
{
	ImGui::Begin("ClickFrame");
	ImGui::Text("ClickFrame:%d",clickTime);
	ImGui::End();

	MyMath::Vector3 pos = MyMathUtility::SplinePosition(points, timeRate, startIndex);

	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;
	timeRate = elapsedTime / maxTime;

	skydomeTrans.Update(camera_);
	groundTrans.Update(camera_);
	boxTrans.Update(camera_);
}

void TitleAnimation::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	boxObj_->Draw(&boxTrans);
	//spriteBack_->Draw(texBack, { 0,0 });
	ClickAnim();
}

void TitleAnimation::Finalize()
{
}

void TitleAnimation::ClickAnim()
{
	MyMath::Vector2 pos = {640,555};

	clickTime++;

	if (clickTime < 60)	//0~30F以内の時
	{
		spriteClickStart_->Draw(texClickStart, { pos.x,pos.y }, { 1,1 }, 0, { 0.5f,0.5f });
	}
	else if (clickTime > 70 && clickTime <= 130)
	{
		spriteClickStart_->Draw(texClickStart, { pos.x,pos.y }, { 1,1 }, 0, { 0.5f,0.5f });
		clickTime = 0;
	}
}
