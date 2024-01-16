#pragma once
#include "BaseScene.h"
#include "SceneManager.h"

#include "AudioManager.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "ObjModel.h"
#include "ObjObject3d.h"

#include "ParticleManager.h"

#include "GPUParticle.h"
#include "WorldTransform.h"

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

private: // メンバ変数
	Input* input_ = nullptr;
	Camera* camera = nullptr;
	
	std::unique_ptr<Light> light;

	//中心地点
	std::unique_ptr<ObjObject3d> colliderObj_;
	std::unique_ptr<ObjModel> colliderModel_;
	WorldTransform colliderTrans;

	std::unique_ptr<GPUParticle> gpuParticle;
	WorldTransform transform;
	
	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;

private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};

