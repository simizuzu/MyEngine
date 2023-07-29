#pragma once
#include "ObjObject3d.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "Light.h"
#include "FbxObject3d.h"
#include "Framework.h"
#include "SceneFactory.h"
#pragma warning(push)
#pragma warning(disable: 4091)
#pragma warning(pop)

// ゲーム全体
class MainGame : public Framework
{
public: // メンバ関数
	MainGame() = default;
	~MainGame() = default;

	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:
	// ゲーム終了フラグ
	bool endRequest_ = false;
};

