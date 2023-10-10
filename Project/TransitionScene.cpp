#include "TransitionScene.h"

void TransitionScene::Initialize()
{
	spriteBlack_ = std::make_unique<Sprite>();
	spriteBlack_->Initialize();
	texBlack_ = TextureManager::Load("Resources/Texture/white1x1.png");

	alpha_ = 1.0f;
	spriteBlack_->SetColor({1.0f,1.0f,1.0f,alpha_});
}

void TransitionScene::Update()
{
	//blackOutTimer--;


}

void TransitionScene::Draw()
{
	
	spriteBlack_->Draw(texBlack_,{100,0},{1280,720});
}

void TransitionScene::Finalize()
{
}
