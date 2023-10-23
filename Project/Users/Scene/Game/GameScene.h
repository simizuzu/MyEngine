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

enum COLOR
{
	red = 1,
	green = 1,
	blue = 1,
	alpha
};

 /**
 * @class GameScene.h
 * @brief GameSceneのクラス
 */

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene
{
public: // メンバ関数
	GameScene() = default;
	~GameScene() = default;
	//　初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;

private:
	/// <summary>
	/// ゲームスタート時の演出
	/// </summary>
	void StartDirection();

	/// <summary>
	/// 上下に黒幕をはる
	/// </summary>
	void BlackMind();

	/// <summary>
	/// タイマーを止めて初期化する処理
	/// </summary>
	void StopTimer();

	/// <summary>
	/// スタート時のムービー演出
	/// </summary>
	void ModelMovie();

private: // メンバ変数
	Input* input_ = nullptr;
	Camera* camera = nullptr;
	std::unique_ptr<Light> light;
	// サウンド
	//AudioManager* audioManager_ = nullptr;
	//uint32_t gameHandle_ = 0;

	Player* player = nullptr;

	//宣言
	INT32 sceneNum = 0;
	char PADING[4]{};

	std::unique_ptr<SceneData> modelData_;
	std::unique_ptr<GameCamera> gameCamera_;
	std::unique_ptr<PostEffect> postEffect_;

	std::unique_ptr<FbxObject3d> robotoObj_;
	std::unique_ptr<FbxModel> robotoModel_;
	
	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;

private: //スタート演出
	float easingFrame = 5.0f;
	float easingTimer = 0.0f;
	float texAlpha = 0.0f;
	float texBlackAlpha = 0.0f;
	uint8_t startCount = 0;

	bool easingFlag = false;
	int8_t pad1[2 ];

	MyMath::Vector2 textureSize = {1280,0};

	MyMath::Vector2 blackUpPos = { 0,0 };
	MyMath::Vector2 blackDownPos = {1280,620};
	MyMath::Vector2 blackSize = {0,100};
	int8_t pad2[4 ];

	std::unique_ptr<Sprite> spriteWhite_;
	std::unique_ptr<Sprite> spriteBlack_;
	std::unique_ptr<Sprite> spriteBlackUp_;
	std::unique_ptr<Sprite> spriteBlackDown_;
	TextureData texWhite_;
	TextureData texBlack_;
	TextureData texBlackUp_;
	TextureData texBlackDown_;

	const float blackTimer_ = 50.0f;
	const int8_t minus1 = -1;
	int8_t pad4[3 ];

private:
	enum class SCENEFASE
	{
		MOVIE,
		BLACKMIND,
		START,
		GAME
	};

	SCENEFASE scene = SCENEFASE::MOVIE;
	int8_t pad3[ 4 ];

private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};

