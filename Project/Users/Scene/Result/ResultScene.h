#pragma once
#include "BaseScene.h"
#include "SceneManager.h"
#include "TransitionScene.h"
#include "TitleAnimation.h"

#include "Input.h"
#include "Light.h"
#include "Sprite.h"
#include "Camera.h"
#include "ObjObject3d.h"
#include "ObjModel.h"

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

	//コンストラクタ・デストラクタ
	ResultScene() = default;
	~ResultScene() = default;

private:
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	SceneManager* sceneManager_ = nullptr;

	std::unique_ptr<TitleAnimation> titleAnimation_;

	std::unique_ptr <TransitionScene> transition_;

	TextureData texClear;
	std::unique_ptr<Sprite> spriteClear;

	WorldTransform trans;
	std::unique_ptr<ObjObject3d> objSkydome;
	std::unique_ptr<ObjModel> modelSkydome;

	//ImGuiデバッグ用
	MyMath::Vector3 cameraPos = { 0,0,0 };
	MyMath::Vector2 pos = { 0,0 };
	MyMath::Vector3 cameraRot;
	MyMath::Vector3 cameraTrans;

	uint8_t blackoutTimer = 60;
	int8_t pad1[ 3 ];

	bool flag = false;
	int8_t pad2[ 7 ];

	//void SetCameraRot(MyMath::Vector3& rotation);

	WorldTransform cameraTransform_;

private:

	//代入演算子削除
	ResultScene& operator=(const ResultScene&) = delete;
	//コピーコンストラクタ削除
	ResultScene(const ResultScene&) = delete;
};

