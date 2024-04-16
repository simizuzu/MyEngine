#include "ClearScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file ClearScene.cpp
 * @brief Clear時の動きに関する処理が書かれてあります
 */

void ClearScene::Initialize(Camera* camera)
{
	assert(camera);
	camera_=camera;

	clearObj_.reset(FbxObject3d::Create());
	clearObj_->SetModel("clear");
	clearObj_->PlayAnimation();
	
	spriteBlackUp = std::make_unique<Sprite>();
	spriteBlackDown = std::make_unique<Sprite>();

	spriteBlackUp->Initialize();
	spriteBlackDown->Initialize();

	texBlackUp = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown = TextureManager::Load("Resources/Texture/black1x1.png");

	clearTrans.Initialize();
	clearTrans.SetScale({ 0.009f,0.009f ,0.009f });

	drawFlag = true;
	sceneTimer = 0;
}

void ClearScene::Update()
{
	sceneTimer++;

	if ( sceneTimer > comparisonNum )
	{
		drawFlag = false;
	}
	camera_->SetEye({ 0.0f,2.0f,3.0f });
	camera_->SetTarget({ 0.0f,2,-10 });

	BlackMind();
	clearTrans.SetTranslation({ modelPos_ });
	clearTrans.Update(camera_);
	clearObj_->Update();

#ifdef _DEBUG
	ImGui::Begin("sceneTimer");
	ImGui::Text("sceneTimer:%d",sceneTimer);
	ImGui::End();
#endif 
}

void ClearScene::Draw()
{
	if ( drawFlag )
	{
		clearObj_->Draw(&clearTrans);
	}

	spriteBlackUp->Draw(texBlackUp,blackUpPos,{ blackSize.x,blackSize.y });
	spriteBlackDown->Draw(texBlackDown,blackDownPos,{ -blackSize.x ,blackSize.y });
}

void ClearScene::GameClear()
{
	
}

void ClearScene::BlackMind()
{
	blackSize.x += blackTimer_;

	if ( blackSize.x > stopBlackSize )
	{
		blackSize.x = stopBlackSize;
	}
}
