#pragma once
#include "LevelLoader.h"
#include "TextureManager.h"
#include "Input.h"

#include "Sprite.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "ObjObject3d.h"
#include "ObjModel.h"

 /**
 * @class GameObject.h
 * @brief GameObjectのクラス
 */

/// <summary>
/// ゲームオブジェクト
/// </summary>
class GameObject
{
public:
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

	/// <summary>
	/// テクスチャ描画
	/// </summary>
	void TexDraw();

	//コンストラクタ・デストラクタ
	GameObject() = default;
	~GameObject() = default;

private:
	Camera* camera_;
	LevelData* curveData2;
	Input* input = nullptr;
	
	TextureData displayTex;
	TextureData reticleTex;
	TextureData UI01Tex;
	TextureData UIRTTex;
	TextureData UILTex;
	std::unique_ptr<Sprite> display;
	std::unique_ptr<Sprite> reticle;
	std::unique_ptr<Sprite> UI01;
	std::unique_ptr<Sprite> UI02;
	std::unique_ptr<Sprite> UIRT;
	std::unique_ptr<Sprite> UIL;

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> bossObj_;

	WorldTransform skydomeTrans;
	WorldTransform groundTrans;
	WorldTransform bossTrans;

	float maxTime = 10.0f;
	float timeRate;
	float targetTimeRate;
	int8_t pad1[ 4 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;
	int8_t pad2[ 4 ];

private:
	//代入演算子削除
	GameObject& operator=(const GameObject&) = delete;
	//コピーコンストラクタ削除
	GameObject(const GameObject&) = delete;
};

