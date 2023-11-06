#include "ClearScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file ClearScene.cpp
 * @brief Clear時の動きに関する処理が書かれてあります
 */

ClearScene* ClearScene::GetInstance()
{
	static ClearScene instance;
	return &instance;
}

void ClearScene::Initialize(Camera* camera)
{
	assert(camera);
	camera_=camera;

	clearModel_=FbxLoader::GetInstance()->LoadModelFromFile("clear");
	clearObj_=FbxObject3d::Create();
	clearObj_->SetModel(clearModel_);
	clearObj_->PlayAnimation();
	clearObj_->SetScale({ 0.009f,0.009f ,0.009f });

	spriteBlackUp = std::make_unique<Sprite>();
	spriteBlackDown = std::make_unique<Sprite>();

	spriteBlackUp->Initialize();
	spriteBlackDown->Initialize();

	texBlackUp = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown = TextureManager::Load("Resources/Texture/black1x1.png");

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
	clearObj_->SetTranslation({ modelPos_ });
	clearObj_->Update(camera_);

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
		clearObj_->Draw();
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
