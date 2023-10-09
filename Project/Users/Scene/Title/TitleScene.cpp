#include "TitleScene.h"

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

#pragma region OBJ

	playerModel.reset(ObjModel::LoadFromObj("Player"));
	playerObj.reset(ObjObject3d::Create());
	playerObj->SetModel(playerModel.get());

	playerTrans.Initialize();
	playerTrans.SetTranslation({0,-3.5f,0});
	playerTrans.SetRotation({ 0,MyMathUtility::degree2Radius * 90,MyMathUtility::degree2Radius * -15 });

	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	if (input_->MouseButtonTrigger(LEFT))
	{
		//sceneManager_->ChangeScene("GAME");
	}
	camera->Update();
	light->Update();

	titleAnimation_->Update();

	rot+=0.05f;
	playerTrans.SetRotation({ 0,rot,MyMathUtility::degree2Radius * -15 });


	playerTrans.Update(camera.get());
}

void TitleScene::Draw()
{
	titleAnimation_->Draw();
	//playerObj->Draw(&playerTrans);
}

void TitleScene::Finalize()
{
}

void TitleScene::ClickBlinking()
{
	
}
