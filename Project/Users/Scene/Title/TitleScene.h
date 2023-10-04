#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "Light.h"



#include "TitleAnimation.h"

#pragma warning(push)
#pragma warning(disable: 4091)
#pragma warning(pop)

// タイトルシーン
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

	/// <summary>
	/// 点滅する処理
	/// </summary>
	void ClickBlinking();

private:
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	SceneManager* sceneManager_= nullptr;

	std::unique_ptr<TitleAnimation> titleAnimation_;
#pragma region OBJ
	std::unique_ptr<ObjObject3d> playerObj;
	std::unique_ptr<ObjModel> playerModel;
	WorldTransform playerTrans;
#pragma endregion


	//ImGuiデバッグ用
	MyMath::Vector3 cameraPos = { 0,0,0 };

	MyMath::Vector2 pos = { 0,0 };

	float rot = 0.0f;
};

