#include "PlayerDamage.h"

void PlayerDamage::Initialize()
{
	takenDamage = std::make_unique<Sprite>();
	takenDamage->Initialize();
	texTakenDamage = TextureManager::Load("Resources/Texture/Scene/takeDamage.png");
	color = { 1.0f,0.0f,0.0f,1.0f };
}

void PlayerDamage::Update()
{
	//デフォルト{1,0,0,0}
	takenDamage->SetColor(color);
}

void PlayerDamage::Draw()
{
	takenDamage->Draw(texTakenDamage,{0,0});
}
