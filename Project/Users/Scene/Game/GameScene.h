#pragma once
#include "BaseScene.h"
#include "SceneManager.h"

#include "Numbers.h"

#include "AudioManager.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "ObjModel.h"
#include "ObjObject3d.h"
#include "Sprite.h"

#include "TextureManager.h"
#include "WorldTransform.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "PostEffect.h"

#include "GameCamera.h"
#include "GameObject.h"

#include "ClearScene.h"
#include "TransitionScene.h"

#include "ParticleManager.h"

#include "EnemyManager.h"
#include "Player.h"

#include "CollisionManager.h"

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
private:
	//1ステージ分の経過時間
	uint8_t gameTimer_ = 0;
	//1s = 60f
	uint8_t oneSecond = 60;
	int8_t pad6[6 ];

private:
	MyMath::Vector2 windowSize = {1280.0f,720.0f};

public: // メンバ関数
	GameScene() = default;
	~GameScene() = default;
	//　初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;

private:
	/// <summary>
	/// ゲームの経過時間
	/// </summary>
	void GameTimer();

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollilsions();

private: // メンバ変数
	Input* input_ = nullptr;
	Camera* camera = nullptr;
	TransitionScene* transition_ = nullptr;
	ClearScene* clearDirection = nullptr;

	std::unique_ptr<Light> light;

	ParticleManager* damageParticle = nullptr;

	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<FbxModel> enemyModel_ = nullptr;

	std::unique_ptr<Player> player_;

	std::unique_ptr<CollisionManager> collisionManager_;

	// サウンド
	//AudioManager* audioManager_ = nullptr;
	//uint32_t gameHandle_ = 0;

	//宣言
	INT32 sceneNum = 0;
	char PADING[4]{};

	std::unique_ptr<GameObject> modelData_;
	std::unique_ptr<GameCamera> gameCamera_;
	std::unique_ptr<PostEffect> postEffect_;

	std::unique_ptr<FbxObject3d> robotoObj_;
	std::unique_ptr<FbxModel> robotoModel_;
	WorldTransform robotoTrans;
	
	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;

private: //スタート演出
	float easingFrame = 5.0f;
	float easingTimer = 0.0f;
	float texAlpha = 0.0f;
	float texBlackAlpha = 0.0f;
	const float stopBlackSize = 1300.0f;

	//上下用座標[x=up,y=down]
	const MyMath::Vector2 stopBlackPos = {200.0f,800.0f};

	//1秒60フレーム
	const float oneSecondFrame = 60.0f;
	float decimalAlpha = 0.0f;
	uint8_t startCount = 0;

	bool easingFlag = false;
	int8_t pad1[2 ];

	MyMath::Vector2 textureSize = {1280,0};

	MyMath::Vector2 blackUpPos = { 0,0 };
	MyMath::Vector2 blackDownPos = {1280,620};
	MyMath::Vector2 blackSize = { 0,100 };
	MyMath::Vector2 fieldNameSize = {50,300};

	std::unique_ptr<Sprite> spriteWhite_;
	std::unique_ptr<Sprite> spriteBlack_;
	std::unique_ptr<Sprite> spriteBlackUp_;
	std::unique_ptr<Sprite> spriteBlackDown_;
	std::unique_ptr<Sprite> spriteStageName01_;

	TextureData texWhite_;
	TextureData texBlack_;
	TextureData texBlackUp_;
	TextureData texBlackDown_;
	TextureData texStageName01_;

	//std::unique_ptr<ObjObject3d> damageObj;
	std::unique_ptr<ObjModel> damageModel;

	const float blackTimer_ = 50.0f;
	const int8_t minus1 = -1;
	int8_t pad4[3 ];

	float cameraTimeRate;
	int8_t pad3[ 4 ];
private:
	enum class SCENEFASE
	{
		MOVIE,
		START,
		GAME,
		RESULT
	};

	SCENEFASE scene = SCENEFASE::MOVIE;
	int8_t pad5[ 4 ];

	DecimalNumbers decimal;
	int8_t pad2[ 4 ];

private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};

