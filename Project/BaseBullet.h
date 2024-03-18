#pragma once
#include "Collider.h"
#include "Camera.h"

class BaseBullet : public Collider
{
public:
	BaseBullet() = default;
	virtual ~BaseBullet() = default;

	virtual void Update(Camera* camera) = 0;

	virtual void Draw() = 0;

	virtual void OnCollision() override = 0;

	virtual MyMath::Vector3 GetCenterPosition() const override = 0;

	bool IsDead() const;

	void OnDead();

	MyMath::Vector3 velocity_;

private:
	bool isDead_ = false;
	int8_t pad[ 3 ];

private:
	//代入演算子削除
	BaseBullet& operator=(const BaseBullet&) = delete;
	//コピーコンストラクタ削除
	BaseBullet(const BaseBullet&) = delete;
};

