#pragma once

#include "ObjObject3d.h"
#include "BaseParticle.h"

/**
 * @class DmageEffect.h
 * @brief BaseParticleを継承したヒット時のパーティクル演出
 */

class DamegeEffect final : public BaseParticle
{
public:
	//コンストラクタ・デストラクタ
	DamegeEffect() = default;
	~DamegeEffect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(ObjModel* model,Camera* camera)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	//3dオブジェクト
	std::unique_ptr<ObjObject3d> obj;
	Camera* camera_ = nullptr;
	WorldTransform transform;
	//std::unique_ptr<ObjModel> model_;
	//加速度
	MyMath::Vector3 accel;
	//移動
	MyMath::Vector3 move;

	//速度
	const float speed = 1.0f;
	//開始時カラー
	float colorStart;
	//終了時カラー
	float colorEnd;
	//開始時透明度
	float alphaStart;
	//終了時透明度
	float alphaEnd;
	int8_t pad1[4 ];

private:
	//代入演算子削除
	DamegeEffect& operator=(const DamegeEffect&) = delete;
	//コピーコンストラクタ削除
	DamegeEffect(const DamegeEffect&) = delete;
};

