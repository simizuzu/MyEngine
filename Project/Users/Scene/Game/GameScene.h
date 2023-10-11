#pragma once
#include "BaseScene.h"
#include "SceneManager.h"

#include "Input.h"
#include "Camera.h"
#include "Light.h"
#include "Sprite.h"
#include "ObjObject3d.h"
#include "ObjModel.h"
#include "AudioManager.h"

#include "TextureManager.h"
#include "WorldTransform.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "PostEffect.h"

#include "SceneData.h"
#include "GameCamera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

class GameScene : public BaseScene
{
public: // メンバ関数
	GameScene() = default;
	~GameScene();
	//　初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;


private: // メンバ変数
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	// サウンド
	//AudioManager* audioManager_ = nullptr;
	//uint32_t gameHandle_ = 0;

	std::unique_ptr<Player> player;

	//宣言
	INT32 sceneNum = 0;
	char PADING[4]{};

	std::unique_ptr<SceneData> modelData_;
	std::unique_ptr<GameCamera> gameCamera_;
	std::unique_ptr<PostEffect> postEffect_;
	
	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};

