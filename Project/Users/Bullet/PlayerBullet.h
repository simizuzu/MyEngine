#pragma once
#include "ObjObject3d.h"
#include "ObjModel.h"
#include "WorldTransform.h"
#include "Collider.h"

 /**
 * @class PlayerBullet.h
 * @brief PlayerBulletのクラス
 */

/// <summary>
/// プレイヤーバレット
/// </summary>
class PlayerBullet final : public Collider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="obj">オブジェクト</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(ObjObject3d* obj, const MyMath::Vector3& position, const MyMath::Vector3 velocity);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool IsDead() const;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	MyMath::Vector3 GetCenterPosition() const override;

	//コンストラクタ・デストラクタ
	PlayerBullet() = default;
	~PlayerBullet() = default;

private:
	//速度
	MyMath::Vector3 velocity_;
	//寿命<frm>
	static const int32_t lifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = lifeTime;
	//デスフラグ
	bool isDead_ = false;
	int8_t pad1[ 7 ];

	//オブジェクト
	ObjObject3d* bulletObj_ = nullptr;

	//トランスフォーム
	WorldTransform bulletTrans_;

private:
	//代入演算子削除
	PlayerBullet& operator=(const PlayerBullet&) = delete;
	//コピーコンストラクタ削除
	PlayerBullet(const PlayerBullet&) = delete;
};

