#include "TitleAnimation.h"
#include "FbxLoader.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file TitleAnimation.cpp
 * @brief Titleの動きに関する処理が書かれてあります
 */

void TitleAnimation::Initalize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	camera_->SetEye({6.0f,3.5f,-7.5f});
	camera_->SetTarget({3.5f,2.0f,0.0f});

	levelData_ = LevelLoader::LoadFile("titleSceneData");

#pragma region Sprite
	texBack = TextureManager::Load("Resources/Texture/Scene/Assault.png");
	texClickStart = TextureManager::Load("Resources/Texture/Scene/clickStart.png");

	spriteBack_ = std::make_unique<Sprite>();
	spriteClickStart_ = std::make_unique<Sprite>();

	spriteBack_->Initialize();
	spriteClickStart_->Initialize();
#pragma endregion

#pragma region Obj
	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("concrete",true));
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

#pragma region Fbx

	pilotModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("pilot"));
	pilotObj_.reset(FbxObject3d::Create());
	pilotObj_->SetModel(pilotModel_.get());
	pilotObj_->PlayAnimation();
	pilotObj_->SetScale({0.0005f,0.0005f ,0.0005f });

#pragma endregion
	rotation = 0;
	translation = 0;
}

void TitleAnimation::Update()
{
#ifdef _DEBUG
	ImGui::Begin("ClickFrame");
	ImGui::Text("ClickFrame:%d",clickTime);
	ImGui::End();
#endif

	rotation += 0.0005f;
	skydomeTrans.SetRotation({0,rotation,0});

	skydomeTrans.Update(camera_);
	groundTrans.Update(camera_);
	robotoTrans.Update(camera_);
	pilotObj_->Update(camera_);
}

void TitleAnimation::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	robotoObj_->Draw(&robotoTrans);
	pilotObj_->Draw();
	spriteBack_->Draw(texBack, { 760,290 },{0.7f,0.7f});
	ClickAnim();
}

void TitleAnimation::Finalize()
{
	clickTime = 0;
}

void TitleAnimation::ClickAnim()
{
	MyMath::Vector2 pos = {640,580};

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
