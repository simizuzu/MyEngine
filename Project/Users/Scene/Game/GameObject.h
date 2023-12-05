#pragma once
#include "LevelLoader.h"
#include "TextureManager.h"

#include "Sprite.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "Player.h"

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

	std::unique_ptr<Player> player_;
	
	TextureData displayTex;
	TextureData reticleTex;
	std::unique_ptr<Sprite> display;
	std::unique_ptr<Sprite> reticle;

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> buildingObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> tyoinoriObj_;

	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> buildingModel_;
	std::unique_ptr<ObjModel> tyoinoriModel_;

	WorldTransform tyoinoriTrans;
	WorldTransform skydomeTrans;
	WorldTransform buildingTrans;
	WorldTransform groundTrans;


	float maxTime = 10.0f;
	float timeRate;
	float targetTimeRate;
	int8_t pad1[ 4 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;
	int8_t pad2[ 4 ];

	std::array<WorldTransform, 100> objs;

private:
	//代入演算子削除
	GameObject& operator=(const GameObject&) = delete;
	//コピーコンストラクタ削除
	GameObject(const GameObject&) = delete;
};

