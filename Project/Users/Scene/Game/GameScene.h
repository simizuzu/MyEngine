#pragma once
#include "BaseScene.h"
#include "SceneManager.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

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
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;

	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;

private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};

