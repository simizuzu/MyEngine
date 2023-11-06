#pragma once

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "TextureManager.h"
#include "Sprite.h"

 /**
 * @class ClearScene.h
 * @brief ClearSceneのクラス
 */

class ClearScene
{
public:
	/// <summary>
	/// インスタンス作成
	/// </summary>
	static ClearScene* GetInstance();

public:
	ClearScene() = default;
	~ClearScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// ゲームクリア時の挙動
	/// </summary>
	void GameClear();

	/// <summary>
	/// 上下の暗幕
	/// </summary>
	void BlackMind();

private:
	const float stopBlackSize = 1300.0f;
	const float blackTimer_ = 50.0f;

	Camera* camera_ = nullptr;

	//モデル
	FbxObject3d* clearObj_ = nullptr;
	FbxModel* clearModel_ = nullptr;

	//テクスチャ
	TextureData texBlackUp;
	TextureData texBlackDown;

	//スプライト
	std::unique_ptr<Sprite> spriteBlackUp;
	std::unique_ptr<Sprite> spriteBlackDown;

	//モデルの座標
	MyMath::Vector3 modelPos_ = {0.0f,0.0f,0.0f};

	MyMath::Vector2 blackUpPos = { 0,0 };
	MyMath::Vector2 blackDownPos = { 1280,620 };
	MyMath::Vector2 blackSize = { 0,100 };
	int8_t pad1[4 ];

	float comparisonNum = 250.0f;
	uint8_t sceneTimer = 0;

	bool drawFlag = true;
	int8_t pad2[2 ];


	//int8_t pad3[7 ];
private:
	//コピーコンストラクタ・代入演算子削除
	ClearScene& operator=(const ClearScene&) = delete;
	ClearScene(const ClearScene&) = delete;
};

