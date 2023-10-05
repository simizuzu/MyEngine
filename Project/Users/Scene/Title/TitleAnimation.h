#pragma once
#include "Camera.h"
#include "LevelLoader.h"

#include "Sprite.h"
#include "ObjObject3d.h"


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
	LevelData* demoCurve_ = nullptr;
	std::unique_ptr<LevelLoader> levelLoader_;

#pragma region Sprite
	TextureData texBack;
	TextureData texClickStart;

	std::unique_ptr<Sprite> spriteBack_;
	std::unique_ptr<Sprite> spriteClickStart_;

#pragma endregion 

#pragma region Obj

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> boxObj_;


	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> boxModel_;

	WorldTransform skydomeTrans;
	WorldTransform groundTrans;
	WorldTransform boxTrans;
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
#pragma endregion

private:
	//代入演算子削除
	TitleAnimation& operator=(const TitleAnimation&) = delete;
	//コピーコンストラクタ削除
	TitleAnimation(const TitleAnimation&) = delete;
};
