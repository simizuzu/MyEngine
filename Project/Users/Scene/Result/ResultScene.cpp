#include "ResultScene.h"
#include "Numbers.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file ResultScene.cpp
 * @brief ResultSceneの処理が書かれてあります
 */

void ResultScene::Initialize()
{
	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	titleAnimation_ = std::make_unique<TitleAnimation>();
	titleAnimation_->Initalize(camera.get());

	transition_ = TransitionScene::GetInstance();
	transition_->Initialize();

	texClear = TextureManager::Load("Resources/Texture/Scene/GameClear.png");
	spriteClear = std::make_unique<Sprite>();
	spriteClear->Initialize();

	modelSkydome.reset(ObjModel::LoadFromObj("skydome",true));
	objSkydome.reset(ObjObject3d::Create());
	objSkydome->SetModel(modelSkydome.get());

	sceneManager_ = SceneManager::GetInstance();

	trans.Initialize();
	trans.SetScale({ 900.0f,900.0f,900.0f });

	cameraTransform_.Initialize();
}

void ResultScene::Update()
{
	transition_->DrawWhiteOut();
	cameraTransform_.Update(camera.get());

	//if ( input_->MouseButtonTrigger(LEFT) )
	//{
	//	blackoutTimer--;
	//}
	//if ( blackoutTimer < 59 )
	//{
	//	blackoutTimer--;
	//	if ( blackoutTimer < 2 )
	//	{
	//		sceneManager_->ChangeScene("TITLE");
	//	}
	//}

	if ( input_->PushKey(DIK_G) )
	{
		cameraRot.x = 0.1f;
		
		camera->SetCameraRot(cameraRot);
	}
	if ( input_->PushKey(DIK_H) )
	{
		cameraRot.x = -0.1f;
		
		camera->SetCameraRot(cameraRot);
	}

	camera->Update();
	light->Update();

	trans.Update(camera.get());

	ImGui::Begin("camera");
	ImGui::SetWindowPos({300,100});
	ImGui::SliderFloat3("target",&camera->target_.x,-500.0f,500.0f);

	ImGui::End();
}

void ResultScene::Draw()
{
//タイトルシーン内の各モデルの描画
	//spriteClear->Draw(texClear,{0,0});

	if ( blackoutTimer < 59 )
	{
		transition_->DrawBlackOut();
	}

	objSkydome->Draw(&trans);
}

void ResultScene::Finalize()
{
	blackoutTimer = 60;
}

//void ResultScene::SetCameraRot(MyMath::Vector3& rotation)
//{
//	MyMath::Vector3 oldTarget = camera->GetTarget();
//
//	MyMath::Vector3 offset = { rotation.x,0,0 };
//
//	offset = MyMath::Vec3Mat4Mul(offset,cameraTransform_.matWorld);
//
//	MyMath::Vector3 target = oldTarget + offset;
//
//	camera->SetTarget(target);
//}
