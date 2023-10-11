#include "TitleScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	titleAnimation_ = std::make_unique<TitleAnimation>();
	titleAnimation_->Initalize(camera.get());

	spriteBlackout = std::make_unique<Sprite>();
	spriteBlackout->Initialize();
	texBlackout = TextureManager::Load("Resources/Texture/Scene/title.png");

	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("blackoutTimer");
	ImGui::Text("blackoutTimer:%d",blackoutTimer);
	ImGui::End();
#endif

	if ( input_->MouseButtonTrigger(LEFT) )
	{
		blackoutTimer--;
	}
	if ( blackoutTimer < 59 )
	{
		blackoutTimer--;
		if ( blackoutTimer < 2 )
		{
			sceneManager_->ChangeScene("GAME");
		}
	}

	camera->Update();
	light->Update();

	titleAnimation_->Update();
}

void TitleScene::Draw()
{
	titleAnimation_->Draw();

	if ( blackoutTimer < 59 )
	{
		spriteBlackout->Draw(texBlackout,{ 0,0 });
	}


}

void TitleScene::Finalize()
{
	blackoutTimer = 60;
}

void TitleScene::TransitionGame()
{

}
