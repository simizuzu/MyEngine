#pragma once
#include "ObjObject3d.h"
#include "ObjModel.h"
#include "WorldTransform.h"
#include "Bullet.h"
#include "Collider.h"
#include "Sprite.h"
#include "TextureManager.h"

 /**
 * @class EnemyBullet.h
 * @brief EnemyBulletのクラス
 */

//クラスの前方宣言
class Player;

class EnemyBullet final : public Collider
{
public:
	//コンストラクタ
	EnemyBullet() = default;
	//デストラクタ
	~EnemyBullet() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="obj">オブジェクト</param>
	/// <param name="pos">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(ObjObject3d* obj,const MyMath::Vector3& pos, const MyMath::Vector3 velocity);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	MyMath::Vector3 GetCenterPosition() const override;

	bool IsDead() const;

	//敵弾に自キャラを渡す
	void SetPlayer(Player* player);

private:

	void DamagePlayerEffect();

private:
	//寿命<frm>
	static const int32_t lifeTime = 60;
	//自キャラ
	Player* player_ = nullptr;
	//デスタイマー
	int32_t deathTimer_ = lifeTime;
	//速度
	MyMath::Vector3 velocity_;
	//デスフラグ
	bool isDead_ = false;
	bool hitFlag = false;
	int8_t pad1[ 6 ];

	//モデル
	//ObjModel* bulletModel_ = nullptr;
	//オブジェクト
	ObjObject3d* bulletObj_ = nullptr;

	//トランスフォーム
	WorldTransform bulletTrans_;

	std::unique_ptr<Sprite> takenDamage;
	TextureData TexTakenDamage;

	//ダメージ用全体スプライト(赤色)
	MyMath::Vector4 color = { 1,0,0,1 };
private:
	//代入演算子削除
	EnemyBullet& operator=(const EnemyBullet&) = delete;
	//コピーコンストラクタ削除
	EnemyBullet(const EnemyBullet&) = delete;
};

