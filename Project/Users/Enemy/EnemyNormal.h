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

/// <summary>
/// ノーマルエネミー
/// </summary>
class EnemyNormal final : public BaseEnemy
{
public:
	//コンストラクタ・デストラクタ
	EnemyNormal() = default;
	~EnemyNormal() = default;

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

public:
	MyMath::Vector3 translation;
	int8_t pad1[4 ];

private:
	Camera* camera_ = nullptr;
	
	std::unique_ptr<FbxObject3d> EnemyObj_;
	FbxModel* EnemyModel_ = nullptr;

private:
	//代入演算子削除
	EnemyNormal& operator=(const EnemyNormal&) = delete;
	//コピーコンストラクタ削除
	EnemyNormal(const EnemyNormal&) = delete;
};

