#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "FPS.h"
#include "AudioManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PostEffect.h"
#include "FbxLoader.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"


// ゲーム全体
class Framework
{
public: // メンバ関数
	virtual ~Framework() = default;
	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Finalize();
	// 毎フレーム更新
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	// 終了チェック
	virtual bool IsEndRequest();

	// 実行
	void Run();

	Framework() = default;

protected:
	// ゲーム終了フラグ
	bool endRequest_ = false;
	int8_t pad1[7 ];

	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	AudioManager* audioManager = nullptr;
	Input* input_ = nullptr;
	FPS* fps_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	std::unique_ptr<PostEffect> postEffect;
	FbxLoader* fbxLoader = nullptr;

	SceneManager* sceneManager_ = nullptr;
	AbstractSceneFactory* sceneFactory_ = nullptr;
private:
	//代入演算子削除
	Framework& operator=(const Framework&) = delete;

	//コピーコンストラクタ削除
	Framework(const Framework&) = delete;
};

