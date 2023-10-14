#pragma once
#include "BaseScene.h"
#include "SceneManager.h"

#include "Input.h"
#include "Light.h"
#include "Sprite.h"
#include "Camera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @class ResultScene.h
 * @brief ResultSceneのクラス
 */

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene : public BaseScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	ResultScene() = default;
	~ResultScene() = default;

private:
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	SceneManager* sceneManager_ = nullptr;

private:

	//代入演算子削除
	ResultScene& operator=(const ResultScene&) = delete;
	//コピーコンストラクタ削除
	ResultScene(const ResultScene&) = delete;
};

