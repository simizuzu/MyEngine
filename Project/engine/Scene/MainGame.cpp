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
	ModelManager::GetInstance()->LoadModel("missile",obj,true);
	ModelManager::GetInstance()->LoadModel("HP_UI",obj);
	ModelManager::GetInstance()->LoadModel("skydome",obj,true);
	ModelManager::GetInstance()->LoadModel("city",obj);
	ModelManager::GetInstance()->LoadModel("gun",obj);
	ModelManager::GetInstance()->LoadModel("concrete",obj,true);

	//fbx
	ModelManager::GetInstance()->LoadModel("roboto",fbx);
	ModelManager::GetInstance()->LoadModel("mob",fbx);
	ModelManager::GetInstance()->LoadModel("clear",fbx);
	ModelManager::GetInstance()->LoadModel("pilot",fbx);

	//texture
	TextureManager::Load("Resources/Texture/Scene/takeDamage.png");
	TextureManager::Load("Resources/Texture/white1x1.png");
	TextureManager::Load("Resources/Texture/black1x1.png");
	TextureManager::Load("Resources/Texture/black1x1.png");
	TextureManager::Load("Resources/Texture/Scene/stagename01.png");
	TextureManager::Load("Resources/Texture/muzzleFlash.png");
	TextureManager::Load("Resources/Texture/reticle.png");

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
