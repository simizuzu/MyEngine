#pragma once

/**
 * @class BaseScene.h
 * @brief 各シーンのための基底クラス
 */

// 前方宣言
class SceneManager;

// シーン基底クラス
class BaseScene
{
public: // メンバ関数

	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

public:
	/// <summary>
	/// シーンをセットする
	/// </summary>
	/// <param name="sceneManager">シーンマネージャ</param>
	virtual void SetSceneManager(SceneManager* sceneManager);

private:
	// シーンマネージャ（借りてくる）
	SceneManager* sceneManager_ = nullptr;
};

