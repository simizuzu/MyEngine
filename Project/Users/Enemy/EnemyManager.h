#pragma once

#include "BaseEnemy.h"
#include "FbxModel.h"
#include "Player.h"
#include "LevelLoader.h"

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
	//敵発生コマンド
	std::stringstream enemyPospCommands;

public:
	static EnemyManager* Create(Player* player, LevelData* data, const std::string& modelName, Camera* camera);

public:
	EnemyManager() = default;
	~EnemyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelName,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Player* player,LevelData* data,const std::string& modelName);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵の数を取得
	/// </summary>
	/// <returns>敵の数</returns>
	const std::list<std::unique_ptr<BaseEnemy>>& GetEnemys();

	MyMath::Vector3 GetEnemyCenterPos();

	bool GetReachCommandFlag();

private:
	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands(Player* player,LevelData* enemyData);

	void UpdateMoveEnemyPop(Player* player,LevelData* enemyData,const std::string& modelName);

	/// <summary>
	/// 敵の削除
	/// </summary>
	void DeleteEnemys();

private:
	//各敵キャラのXYZ座標
	MyMath::Vector3 enemysPos_;
	//外部から読み込んで入れる変数
	float waitTime;
	//待機用タイム変数(現在進行形)
	float waitTimer;
	//コマンドが最後までたどり着いたかのフラグ
	bool reachCommandFlag = false;
	//待っているかのフラグ
	bool waitFlag = true;
	int8_t pad[2 ];

	Camera* camera_ = nullptr;
	std::string modelName_;

private:
	//代入演算子削除
	EnemyManager& operator=(const EnemyManager&) = delete;
	//コピーコンストラクタ削除
	EnemyManager(const EnemyManager&) = delete;
};

