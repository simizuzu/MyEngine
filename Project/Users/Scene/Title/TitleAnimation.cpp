#include "TitleAnimation.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

void TitleAnimation::Initalize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	camera_->SetEye({6.0f,2.5f,-6.5f});
	camera_->SetTarget({3.5f,2.0f,0.0f});

	levelData_ = LevelLoader::LoadFile("titleSceneData");;

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
	groundModel_.reset(ObjModel::LoadFromObj("concrete"));
	robotoModel_.reset(ObjModel::LoadFromObj("roboto"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	robotoObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	groundObj_->SetModel(groundModel_.get());
	robotoObj_->SetModel(robotoModel_.get());

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	robotoTrans.Initialize();

	float scale = 900.0f;
	skydomeTrans.SetScale({ scale,scale,scale });
	//.SetScale({50.0f,50.0f ,50.0f });
#pragma endregion
}

void TitleAnimation::Update()
{
	ImGui::Begin("ClickFrame");
	ImGui::Text("ClickFrame:%d",clickTime);
	ImGui::End();


	skydomeTrans.Update(camera_);
	groundTrans.Update(camera_);
	robotoTrans.Update(camera_);
}

void TitleAnimation::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	robotoObj_->Draw(&robotoTrans);
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
