#include "MainGame.h"
#include "ModelManager.h"

 /**
 * @file MainGame.cpp
 * @brief MainGameの処理について書いてあります
 */

void MainGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();

	//obj
	ModelManager::GetInstance()->LoadModel("skydome",obj,true);

	//fbx
	ModelManager::GetInstance()->LoadModel("KaedeCamera",fbx);

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
	sceneManager_->Draw();
}
