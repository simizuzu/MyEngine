#pragma once
#include <stdint.h>
#include "Vector3.h"

 /**
 * @class Collider.h
 * @brief 衝突判定オブジェクトクラス
 */
class Collider
{
private:
	//半径
	float radius_ = 1.5f;
	int8_t pad1[4 ];
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision();

	//中心座標を取得(純粋仮想関数)
	virtual MyMath::Vector3 GetCenterPosition() const = 0;

public:
	//衝突判定を取得する
	float GetRadius();
	//衝突判定を設定する
	void SetRadius(float radius);

public:
	Collider() = default;
	virtual ~Collider() = default;

private:
	//代入演算子削除
	Collider& operator=(const Collider&) = delete;
	//コピーコンストラクタ削除
	Collider(const Collider&) = delete;
};

