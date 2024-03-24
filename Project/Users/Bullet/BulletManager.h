#pragma once
#include "BaseBullet.h"
#include "ObjObject3d.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <string>
MYENGINE_SUPPRESS_WARNINGS_END

//前方宣言
class Player;

class BulletManager
{
public:
	BulletManager() = default;
	~BulletManager() = default;

	//インスタンス
	static BulletManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelName,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 通常弾の生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="velocity">速度</param>
	void CreateNormalBullet(const MyMath::Vector3& pos,const MyMath::Vector3 velocity,Player* player);

	//弾リストを削除
	void DeleteBullets();

	//void SetPlayer(Player* player);

public:
	//通常弾のリストを取得
	const std::list<std::unique_ptr<BaseBullet>>& GetNormalBullets();

private:
	//通常弾のリスト
	std::list<std::unique_ptr<BaseBullet>> normalBullets;

private:
	Camera* camera_ = nullptr;
	std::string modelName_;

private:
	//代入演算子削除
	BulletManager& operator=(const BulletManager&) = delete;
	//コピーコンストラクタ削除
	BulletManager(const BulletManager&) = delete;
};

