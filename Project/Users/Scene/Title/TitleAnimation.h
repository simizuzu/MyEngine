#pragma once
#include "Camera.h"
#include "LevelLoader.h"

#include "Sprite.h"
#include "ObjObject3d.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include <map>

 /**
 * @class TitleAnimation.h
 * @brief TitleAnimationのクラス
 */

/// <summary>
/// タイトルのアニメーション
/// </summary>
class TitleAnimation
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	//コンストラクタ・デストラクタ
	TitleAnimation() = default;
	~TitleAnimation() = default;

private:
	/// <summary>
	/// クリックするテクスチャのアニメーション処理
	/// </summary>
	void ClickAnim();

private: //メンバ変数
	//カメラ
	Camera* camera_ = nullptr;
	LevelData* levelData_ = nullptr;

#pragma region Sprite
	TextureData texBack;
	TextureData texClickStart;

	std::unique_ptr<Sprite> spriteBack_;
	std::unique_ptr<Sprite> spriteClickStart_;

#pragma endregion 

#pragma region Obj

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> robotoObj_;


	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> robotoModel_;

	WorldTransform skydomeTrans;
	WorldTransform groundTrans;
	WorldTransform robotoTrans;

	std::map<std::string, std::unique_ptr<ObjModel>>models;
	std::vector<std::unique_ptr<ObjObject3d>> objects;
#pragma endregion

#pragma region fbx

	std::unique_ptr<FbxObject3d> pilotObj_;
	std::unique_ptr<FbxModel> pilotModel_;

#pragma endregion

#pragma region その他変数
	
	uint16_t clickTime = 0;
	int8_t pad1[ 6 ];

	std::vector<LevelData::CurveData> points;

	float maxTime = 0.5f;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
	int8_t pad2[ 4 ];

	float targetTimeRate;
	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;
	int8_t pad3[ 4 ];

	float rotation;
	int8_t pad4[4 ];

	float translation;
	int8_t pad5[ 4 ];

#pragma endregion

private:
	//代入演算子削除
	TitleAnimation& operator=(const TitleAnimation&) = delete;
	//コピーコンストラクタ削除
	TitleAnimation(const TitleAnimation&) = delete;
};

