#pragma once
#include "BaseBullet.h"
#include "ObjObject3d.h"

class NormalBullet : public BaseBullet
{
public:
	NormalBullet() = default;
	~NormalBullet() = default;

	void Initialize(ObjObject3d* obj, const std::string filePath,const MyMath::Vector3& pos,const MyMath::Vector3 velocity,MyMath::Vector3& target,float bulletSpeed);

	void Update(Camera* camera) override;

	void Draw() override;

private:
	//オブジェクト
	ObjObject3d* bulletObj_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform bulletTrans_;

	//狙う対象
	MyMath::Vector3 target_;
	//寿命<frm>
	static const int32_t lifeTime = 60 * 10;
		//弾の速度
	float bulletSpeed_;
	//デスタイマー
	int32_t deathTimer_ = lifeTime;
	int8_t pad1[ 4 ];

private:
	//代入演算子削除
	NormalBullet& operator=(const NormalBullet&) = delete;
	//コピーコンストラクタ削除
	NormalBullet(const NormalBullet&) = delete;
};

