#pragma once
#include "ObjModel.h"
#include "Camera.h"
#include "ParticleGrain.h"

/**
 * @file BaseParticle.h
 * @brief パーティクル基底クラス
 */

class BaseParticle
{
public:
	//コンストラクタ・デストラクタ
	BaseParticle() = default;
	virtual ~BaseParticle() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	virtual void Initialize(ObjModel* model,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// デスフラグの取得
	/// </summary>
	virtual bool GetIsDead() const;

	//パーティクルの一粒
	ParticleGrain oneGrain;

protected:
	ObjModel* model_;
	Camera* camera_;
	bool isDead = false;
	int8_t pad[7 ];
private:

	//代入演算子削除
	BaseParticle& operator=(const BaseParticle&) = delete;
	//コピーコンストラクタ削除
	BaseParticle(const BaseParticle&) = delete;
};

