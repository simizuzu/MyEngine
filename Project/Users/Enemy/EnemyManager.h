#pragma once

#include "BaseEnemy.h"
#include "FbxModel.h"

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
	void Update(Player* player);

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

	bool waitFlag = false;
	int8_t pad1[7 ];

	//csvから読み込んで入れる変数
	int32_t waitTime;
	//待機用タイム変数(現在進行形)
	int32_t waitTimer;
	int8_t pad2[ 4 ];

	Camera* camera_ = nullptr;
	FbxModel* model_ = nullptr;

private:
	//代入演算子削除
	EnemyManager& operator=(const EnemyManager&) = delete;
	//コピーコンストラクタ削除
	EnemyManager(const EnemyManager&) = delete;
};

