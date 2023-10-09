#include "MainGame.h"

void MainGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();
	//3Dオブジェクト静的初期化(Obj版)
	ObjObject3d::StaticInitialize(dxCommon_->GetDevice());
	//ライト静的初期化
	Light::StaticInitialise(dxCommon_);
	//スプライト静的初期化
	Sprite::StaticInitialize();
	//パーティクルマネージャー静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice(), dxCommon_->GetCommandList());
	//3Dオブジェクト静的初期化(Fbx版)
	FbxObject3d::StaticInitialize(dxCommon_->GetDevice());

	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MainGame::Finalize()
{
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MainGame::Update()
{
	// 基底クラスの更新処理
	Framework::Update();
}

void MainGame::Draw()
{
	// ゲームシーンの描画
	//baseScene->Draw();
	sceneManager_->Draw();
}
