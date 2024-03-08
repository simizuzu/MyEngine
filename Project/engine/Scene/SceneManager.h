#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/**
 * @class SceneManager.h
 * @brief 各シーンを管理するクラス
 */

/// <summary>
/// シーンマネージャ
/// </summary>
class SceneManager final
{
public: // メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	// 次のシーン予約
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);
	void ChangeScene(const std::string& sceneName);

	//シングルトン
	static SceneManager* GetInstance();

private:
	// 今のシーン（実行中シーン）
	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	//コピーコンストラクタ削除
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&) = delete;
};

