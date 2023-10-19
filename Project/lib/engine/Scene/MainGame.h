#pragma once
#include "ObjObject3d.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "Light.h"
#include "FbxObject3d.h"
#include "PostEffect.h"
#include "Framework.h"
#include "SceneFactory.h"

/**
 * @class MainGame.h
 * @brief 自作したクラスの初期化をまとめて呼び出しするクラス
 */

// ゲーム全体
class MainGame : public Framework
{
public: // メンバ関数
	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

	//コンストラクタ・デストラクタ
	MainGame() = default;
	~MainGame() = default;

private:
	// ゲーム終了フラグ
	bool endRequest_ = false;
	int8_t pad[7 ];

	//代入演算子削除
	MainGame& operator=(const MainGame&) = delete;
	//コピーコンストラクタ削除
	MainGame(const MainGame&) = delete;

};

