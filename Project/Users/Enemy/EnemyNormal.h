#pragma once
#include "BaseEnemy.h"

#include "Camera.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

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

	//弾リストの取得
	const std::list<EnemyBullet*>& GetBullets() const override;

	//プレイヤーのポインタをセット
	void SetPlayer(Player* player);

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
	/// こっちに振り向く処理
	/// </summary>
	void LookMe();

	//ワールド座標の原点
	MyMath::Vector3 GetCenterPosition() const override;

private:
	Camera* camera_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;

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

	float enemyAngle;

	//敵のHP
	uint8_t enemyHP;

	bool isDead = false;
	bool bulletIntervalFlag = false;
	const uint8_t resetTimer = 20;
	uint8_t bulletIntervalTimer;
	bool flag = false;
	int8_t pad1[ 2 ];

public:
	MyMath::Vector3 translation;

private:
	//代入演算子削除
	EnemyNormal& operator=(const EnemyNormal&) = delete;
	//コピーコンストラクタ削除
	EnemyNormal(const EnemyNormal&) = delete;
};

