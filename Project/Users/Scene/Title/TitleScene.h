#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "Light.h"
#include "SceneManager.h"

#include "TitleAnimation.h"

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

	//FBXモデルデータ
	std::unique_ptr<FbxObject3d> object;
	WorldTransform transform;

	//キーフレームデータ
	Keyframe* keyframeData;
	float animTime = 1.0f / 60.0f;
	MyMath::Vector3 translate;
	MyMath::Quaternion rotate;

private:

	//代入演算子削除
	TitleScene& operator=(const TitleScene&) = delete;
	//コピーコンストラクタ削除
	TitleScene(const TitleScene&) = delete;
};

