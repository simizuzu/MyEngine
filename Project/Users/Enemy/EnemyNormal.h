#pragma once
#include "BaseEnemy.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "BulletManager.h"

/**
 * @class EnemyNormal.h
 * @brief 敵キャラの派生クラス（通常の敵キャラ）
 */

 //クラスの前方宣言
 class Player;

/// <summary>
/// ノーマルエネミー
/// </summary>
class EnemyNormal final : public BaseEnemy
{
public:
	//コンストラクタ・デストラクタ
	EnemyNormal() = default;
	~EnemyNormal();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(const std::string& filePath,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	////弾リストの取得
	//const std::list<EnemyBullet*>& GetBullets() const override;

	//プレイヤーのポインタをセット
	void SetPlayer(Player* player) override;

	void SetEnemyTranslation(MyMath::Vector3 translation);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;
	//弾を受けたときの処理
	void HitBullet() override;
	//死んでいるかどうか
	bool IsDead() const override;

private:
	/// <summary>
	///	弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 振り向き処理
	/// </summary>
	void Turn();
	
	//ワールド座標の原点
	MyMath::Vector3 GetCenterPosition() const override;

private:
	//カメラ
	Camera* camera_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;

	//バレットマネージャ
	BulletManager* bulletManager_ = nullptr;
	std::unique_ptr<ParticleManager> hitEffect;

	//弾の速度
	const float bulletSpeed = 0.8f;
	MyMath::Vector3 velocity = {0.0f,0.0f,bulletSpeed};

	//自キャラのワールド座標
	MyMath::Vector3 playerWorldPos;
	//敵キャラのワールド座標
	MyMath::Vector3 enemyWorldPos;
	//敵キャラ→自キャラの差分ベクトル
	MyMath::Vector3 enemyToPlayerVec;
	//正規化するベクトル
	MyMath::Vector3 enemyDir;

	//敵のモデル
	std::unique_ptr<FbxObject3d> enemyObj_;
	std::string modelName;
	WorldTransform enemyTrans;

	//弾のモデル
	std::unique_ptr<ObjObject3d> bulletObj;

	//体力バーのモデル
	std::unique_ptr<ObjObject3d> HP_UIObj;
	WorldTransform HP_UITrans;

	MyMath::Vector3 UITranslation;
	MyMath::Vector3 HPScale;

	//敵1体の座標
	MyMath::Vector3 translation_;

	float enemyAngleY;
	float enemyAngleX;

	//敵のHP
	uint8_t enemyHP;

	bool isDead = false;
	bool isFire = false;
	const uint8_t resetTimer = 20;
	uint8_t bulletIntervalTimer;
	bool turnFlag = false;
	int8_t pad1[ 6 ];

private:
	//代入演算子削除
	EnemyNormal& operator=(const EnemyNormal&) = delete;
	//コピーコンストラクタ削除
	EnemyNormal(const EnemyNormal&) = delete;
};

