#include "TitleAnimation.h"
#include "FbxLoader.h"
#include "Numbers.h"

#include "ModelManager.h"

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

	camera_->SetEye({ 6.0f,3.5f,-7.5f });
	camera_->SetTarget({ 3.5f,2.0f,2.0f });

#pragma region Sprite
	texBack = TextureManager::Load("Resources/Texture/Scene/Assault.png");
	texClickStart = TextureManager::Load("Resources/Texture/Scene/clickStart.png");

	spriteBack_ = std::make_unique<Sprite>();
	spriteClickStart_ = std::make_unique<Sprite>();

	spriteBack_->Initialize();
	spriteClickStart_->Initialize();
#pragma endregion

#pragma region Obj
	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	robotoObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel("skydome",true);
	groundObj_->SetModel("concrete",true);
	robotoObj_->SetModel("roboto");

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	robotoTrans.Initialize();
	pilotTrans.Initialize();

	skydomeTrans.SetScale({ skydomeScale,skydomeScale,skydomeScale });
#pragma endregion

#pragma region Fbx

	pilotObj_.reset(FbxObject3d::Create());
	pilotObj_->SetModel("pilot");
	pilotObj_->PlayAnimation();

	pilotTrans.SetScale({ 0.006f,0.006f ,0.006f });
	pilotTrans.SetTranslation({ -0.9f,0.0f,-1.2f });

#pragma endregion
	rotation = zero;
	translation = zero;

	keyframeData = LevelLoader::LoadKeyframe("anim",animationTime);
}

void TitleAnimation::Update()
{
#ifdef _DEBUG
	ImGui::Begin("ClickFrame");
	ImGui::Text("ClickFrame:%d",clickTime);
	ImGui::End();
#endif

	animationTime += 1.0f / 10.0f;
	animationTime = std::fmod(animationTime,keyframeData->meshKeyframe[ "Cube" ].duration);

	translate = MyMathUtility::CalculateValueLerp(keyframeData->meshKeyframe[ "Cube" ].scale,animationTime);

	rotation += desimalRot;
	skydomeTrans.SetRotation({ 0,rotation,0 });

	robotoTrans.SetScale(translate);

	skydomeTrans.Update(camera_);
	groundTrans.Update(camera_);
	robotoTrans.Update(camera_);
	pilotTrans.Update(camera_);
	pilotObj_->Update();
}

void TitleAnimation::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	robotoObj_->Draw(&robotoTrans);
	pilotObj_->Draw(&pilotTrans);
	spriteBack_->Draw(texBack,black1x1Size,scale,texRot,anchorpoint);
	ClickAnim();
}

void TitleAnimation::Finalize()
{
	clickTime = zero;
}

void TitleAnimation::ClickAnim()
{
	MyMath::Vector2 pos = { 640,580 };

	clickTime++;

	if ( clickTime < 60 )	//0~60F以内の時
	{
		spriteClickStart_->Draw(texClickStart,{ pos.x,pos.y },scale,texRot,anchorpoint);
	}
	else if ( clickTime > 70 && clickTime <= 130 ) //70~130F以内の時
	{
		spriteClickStart_->Draw(texClickStart,{ pos.x,pos.y },scale,texRot,anchorpoint);
		clickTime = zero;
	}
}
