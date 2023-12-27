#pragma once
#include "SuppressWarning.h"
#include "ObjObject3d.h"
#include "ObjModel.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class Bullet.h
 * @brief 弾の基底クラス
 */

class Bullet
{
public:
	Bullet() = default;
	virtual ~Bullet() = default;

	virtual void Initialize(ObjModel* model) = 0;

	virtual void Set(const MyMath::Vector3& pos, const MyMath::Vector3 velocity);

	virtual void Update(Camera* camera);

	virtual void Draw();

	virtual bool GetIsDead() const;

	virtual void SetIsDead(bool isDead);

	virtual void OnCollision();

	virtual MyMath::Vector3 GetWorldPos();

protected:
	//速度
	MyMath::Vector3 velocity_;
	//寿命<frm>
	static const int32_t lifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = lifeTime;

	bool isDead_ = false;

	int8_t pad1[ 7 ];

	std::unique_ptr<ObjObject3d> obj_;

	ObjModel* model_ = nullptr;

	WorldTransform bulletTrans_;

private:
	//代入演算子削除
	Bullet& operator=(const Bullet&) = delete;
	//コピーコンストラクタ削除
	Bullet(const Bullet&) = delete;
};

