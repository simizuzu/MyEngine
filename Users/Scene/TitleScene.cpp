#include "TitleScene.h"

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

#pragma region Sprite
	tex = TextureManager::Load("Resources/Texture/title.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();

	tex2 = TextureManager::Load("Resources/Texture/warning.png");
	warningSprite_ = std::make_unique<Sprite>();
	warningSprite_->Initialize();
#pragma endregion

#pragma region OBJ

	skydome = std::make_unique<ObjModel>();
	skydome.reset(ObjModel::LoadFromObj("skydome"));
	skydomeObj.reset(ObjObject3d::Create());
	skydomeObj->SetModel(skydome.get());

	camera->SetEye({ 0.0f, 0.0f, 0.0f });

	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	camera->Update();
	light->Update();
}

void TitleScene::Draw()
{

}

void TitleScene::Finalize()
{
}
