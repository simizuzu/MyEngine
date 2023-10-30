#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "Light.h"

#include "TitleAnimation.h"
#include "TransitionScene.h"

 /**
 * @class TitleScene.h
 * @brief TitleSceneのクラス
 */

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene
{
public: // メンバ関数

	TitleScene() = default;
	~TitleScene() = default;

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

private:
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	SceneManager* sceneManager_= nullptr;

	std::unique_ptr<TitleAnimation> titleAnimation_;

	TransitionScene* transition_ = nullptr;

	//ImGuiデバッグ用
	MyMath::Vector3 cameraPos = { 0,0,0 };
	MyMath::Vector2 pos = { 0,0 };

	uint8_t blackoutTimer = 60;
	int8_t pad1[3 ];	

	std::unique_ptr<Sprite> spriteBlackout;
	TextureData texBlackout;

private:

	//代入演算子削除
	TitleScene& operator=(const TitleScene&) = delete;
	//コピーコンストラクタ削除
	TitleScene(const TitleScene&) = delete;
};

