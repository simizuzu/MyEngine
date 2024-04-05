#include "Framework.h"
#include "ObjModel.h"
#include "ModelManager.h"
 /**
 * @file Framework.cpp
 * @brief Frameworkの処理について書いてあります
 */

void Framework::Initialize()
{
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audioManager = AudioManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	imGuiManager = ImGuiManager::GetInstance();
	fbxLoader = FbxLoader::GetInstance();

	// WindowsAPI初期化
	winApp_->Initialize();
	// FPS固定初期化
	fps_ = new FPS();
	fps_->InitializeFixFps();
	//// DirectX初期化
	dxCommon_->Initialize();
	// スプライト共通部の初期化
	textureManager_->Initialize(dxCommon_);
	// Audio初期化
	audioManager->Initialize();
	//// ImGui初期化
	imGuiManager->Initialize(winApp_, dxCommon_);
	//// Input初期化
	input_->Initialize();
	//FBXLoaderの初期化
	fbxLoader->Initialize(dxCommon_->GetDevice());
	//ポストエフェクト初期化
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize(dxCommon_->GetDevice());

	//3Dオブジェクト静的初期化(Obj版)
	ObjObject3d::StaticInitialize(dxCommon_->GetDevice());
	//ライト静的初期化
	Light::StaticInitialise(dxCommon_);
	//スプライト静的初期化
	Sprite::StaticInitialize();
	//3Dオブジェクト静的初期化(Fbx版)
	FbxObject3d::StaticInitialize(dxCommon_->GetDevice());

	sceneManager_ = SceneManager::GetInstance();
}

void Framework::Finalize()
{
	sceneManager_->Finalize();
	delete sceneFactory_;

	ObjObject3d::Finalize();
	Sprite::Finalize();
	FbxObject3d::Finalize();
	ObjModel::Finalize();
	ModelManager::Finalze();
	postEffect->Finalize();
	postEffect.reset();
	// ImGui解放
	imGuiManager->Finalize();
	// テクスチャマネージャ解放
	textureManager_->Delete();
	//// オーディオマネージャー初期化
	audioManager->Destroy();
	//FBXLoaderの後始末
	fbxLoader->Finalize();
	dxCommon_->Finalize();
	// WindowsAPIの終了処理
	winApp_->Finalize();
	// WinApp解放
	winApp_->Delete();
	// FPS解放
	delete fps_;
}

void Framework::Update()
{
	// Windowsのメッセージ処理
	if (winApp_->ProccessMessage())
	{
		endRequest_ = true;
	}
	// 入力の更新
	input_->Update();
	audioManager->Update();

	/// ImGui更新処理開始
	imGuiManager->Begin();
	// ゲームシーンの毎フレーム処理
	sceneManager_->Update();
	// ImGui更新処理終了
	imGuiManager->End();
}

void Framework::Run()
{
	// ゲームの初期化
	Initialize();

	while (true) // ゲームループ
	{
		// 毎フレーム更新
		Update();
		// 終了リクエストが来たら抜ける
		if (IsEndRequest())
		{
			break;
		}
		/*postEffect->PreDrawScene(dxCommon_->GetCommandList());
		postEffect->Draw(dxCommon_->GetCommandList());
		postEffect->PostDrawScene(dxCommon_->GetCommandList());*/

		dxCommon_->PreDraw(winApp_);
		Draw();
		//ImGui描画
		imGuiManager->Draw(dxCommon_);
		dxCommon_->PostDraw();
		// FPS固定更新
		fps_->UpdateFixFPS();
	}
	// ゲーム終了
	Finalize();
}

bool Framework::IsEndRequest()
{
	return endRequest_;
}
