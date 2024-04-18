#include "PlayerAppearScene.h"
#include "BossAppearScene.h"

void PlayerAppearScene::Initialize()
{
	robotoObj_.reset(FbxObject3d::Create());
	robotoObj_->SetModel("roboto");
	robotoObj_->PlayAnimation();

	robotoTrans.Initialize();
	robotoTrans.SetScale({ 0.009f,0.009f ,0.009f });

	spriteStageName01_ = std::make_unique<Sprite>();
	spriteStageName01_->Initialize();
	texStageName01_ = TextureManager::Load("Resources/Texture/Scene/stagename01.png");
}

void PlayerAppearScene::Update(Animation* pState,Camera* camera)
{
	camera->SetEye({ -2.2f,1,-4 });
	camera->SetTarget({ -3,2,0 });
	BlackMind();
	ModelMovie();

	robotoTrans.Update(camera);
	robotoObj_->Update();

	if ( startCount > 250 )
	{
		texBlackAlpha = 1.0f;
		startCount = 0;
		pState->ChangeState(new BossAppearScene());
	}
}

void PlayerAppearScene::Draw(Animation* pState)
{
	spriteStageName01_->Draw(texStageName01_,fieldNameSize);
	robotoObj_->Draw(&robotoTrans);
}

void PlayerAppearScene::BlackMind()
{
	blackSize.x += blackTimer_;

	if ( blackSize.x > stopBlackSize )
	{
		blackSize.x = stopBlackSize;
	}
}

void PlayerAppearScene::ModelMovie()
{
	startCount++;

	if ( startCount > 170 )
	{
		texBlackAlpha += 0.1f;
	}
}