#pragma once

#include "BaseEnemy.h"
#include "FbxModel.h"
#include "Player.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <list>
#include <memory>
#include <sstream>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class EnemyManager.h
 * @brief 敵キャラのマネージャクラス
 */

/// <summary>
/// エネミーマネージャ
/// </summary>
class EnemyManager
{
private:
	//敵の数
	std::list<std::unique_ptr<BaseEnemy>>enemys;
	//敵の弾
	std::list<EnemyBullet*> bullets;

	//敵発生コマンド
	std::stringstream enemyPospCommands;

public:
	static EnemyManager* Create(const std::string& filePath, FbxModel* model, Camera* camera);

public:
	EnemyManager() = default;
	~EnemyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(FbxModel* model,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 各敵キャラの初期化情報(更新処理の先頭にいれる)
	/// </summary>
	void EnemyNormalEmit(Player* player);

	/// <summary>
	/// 敵の数を取得
	/// </summary>
	/// <returns>敵の数</returns>
	const std::list<std::unique_ptr<BaseEnemy>>& GetEnemys();

	bool GetReachCommandFlag();


	//const std::list<std::unique_ptr<BaseEnemy>>& GetEnemyBullets();

private:
	/// <summary>
	/// 敵発生データの読み込み("Resources/filePath/fname.csv")
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadEnemyPopData(const std::string& filePath);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands(Player* player);

private:
	//各敵キャラのXYZ座標
	MyMath::Vector3 enemysPos_;
	uint8_t pad1[4 ];

	//敵の合計数
	size_t enemyCount;

	//待っているかのフラグ
	bool waitFlag = false;
	//コマンドが最後までたどり着いたかのフラグ
	bool reachCommandFlag = false;
	uint8_t pad2[ 2 ];

	//csvから読み込んで入れる変数
	int32_t waitTime;
	//待機用タイム変数(現在進行形)
	int32_t waitTimer;
	uint8_t pad3[ 4 ];

	Camera* camera_ = nullptr;
	FbxModel* model_ = nullptr;

private:
	//代入演算子削除
	EnemyManager& operator=(const EnemyManager&) = delete;
	//コピーコンストラクタ削除
	EnemyManager(const EnemyManager&) = delete;
};

