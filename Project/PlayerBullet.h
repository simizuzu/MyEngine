#pragma once
#include "BaseBullet.h"

 /**
 * @class PlayerBullet.h
 * @brief PlayerBulletのクラス
 */
class PlayerBullet final : public BaseBullet
{
	//コンストラクタ
	PlayerBullet() = default;
	//デストラクタ
	~PlayerBullet() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <param name="pos">発射座標</param>
	/// <param name="velocity">弾速度</param>
	void Initialize(const std::string& modelName, const MyMath::Vector3& pos, const MyMath::Vector3 velocity);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	MyMath::Vector3 GetCenterPosition() const override;

	bool IsDead() const override;

	bool IsHit() const;

private:
	//寿命<frm>
	static const int32_t lifeTime = 60 * 5;

private:
	//代入演算子削除
	PlayerBullet& operator=(const PlayerBullet&) = delete;
	//コピーコンストラクタ削除
	PlayerBullet(const PlayerBullet&) = delete;
};

