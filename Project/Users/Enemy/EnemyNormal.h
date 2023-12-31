#pragma once
#include "BaseEnemy.h"

#include "Camera.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "EnemyBullet.h"

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
	void Initialize(FbxModel* model,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	//const std::list<EnemyBullet*>& GetBullets() const;

	void SetPlayer(Player* player);

private:
	/// <summary>
	///	弾発射
	/// </summary>
	void Fire();

	MyMath::Vector3 GetCenterPosition() const override;

public:
	MyMath::Vector3 translation;
	int8_t pad1[4 ];

private:
	Camera* camera_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;

	MyMath::Vector3 worldPosition;
	int8_t pad3[ 4 ];

	//敵のモデル
	std::unique_ptr<FbxObject3d> enemyObj_;
	FbxModel* enemyModel_ = nullptr;
	WorldTransform enemyTrans;

	//当たり判定モデル
	std::unique_ptr<ObjObject3d> colliderObj_;
	std::unique_ptr<ObjModel> colliderModel_;
	WorldTransform colliderTrans;

	//弾のモデル
	std::unique_ptr<ObjObject3d> bulletObj;
	std::unique_ptr<ObjModel> bulletModel;

	//弾のリスト
	std::list<EnemyBullet*> bullets;
	bool bulletIntervalFlag = false;
	const uint8_t resetTimer = 20;
	uint8_t bulletIntervalTimer;
	bool flag = false;
	int8_t pad2[ 4 ];

private:
	//代入演算子削除
	EnemyNormal& operator=(const EnemyNormal&) = delete;
	//コピーコンストラクタ削除
	EnemyNormal(const EnemyNormal&) = delete;
};

