#pragma once

#include "Enemy.h"

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
	std::list<std::unique_ptr<Enemy>>enemys;

	//敵発生コマンド
	std::stringstream enemyPospCommands;

public:
	EnemyManager() = default;
	~EnemyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 各敵キャラの初期化情報
	/// </summary>
	void EnemyNormalEmit();

private:
	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

private:
	//各敵キャラのXYZ座標
	MyMath::Vector3 enemysPos_;

	bool waitFlag = false;
	int8_t pad1[7 ];

	//csvから読み込んで入れる変数
	int32_t waitTime;
	//待機用タイム変数(現在進行形)
	int32_t waitTimer;
	int8_t pad2[ 4 ];

private:
	//代入演算子削除
	EnemyManager& operator=(const EnemyManager&) = delete;
	//コピーコンストラクタ削除
	EnemyManager(const EnemyManager&) = delete;
};

