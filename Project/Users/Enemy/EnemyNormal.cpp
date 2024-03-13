#include "EnemyNormal.h"
#include "Numbers.h"
#include "Player.h"
#include "ModelManager.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file EnemyNormal.cpp
 * @brief EnemyNormalの処理が書かれてあります
 */

EnemyNormal::~EnemyNormal()
{
	//敵の弾を削除
	for ( EnemyBullet* bullet : bullets )
	{
		delete bullet;
	}
}

void EnemyNormal::Initialize(const std::string& filePath,Camera* camera)
{
	//渡されたかチェック
	assert(camera);
	camera_ = camera;
	modelName = filePath;

	//雑魚敵のモデル
	enemyObj_.reset(FbxObject3d::Create());
	enemyObj_->SetModel(modelName);
	enemyObj_->PlayAnimation();

	//雑魚敵のワールド座標
	enemyTrans.Initialize();
	enemyTrans.SetScale({ 0.04f,0.04f ,0.04f });

	//自機狙い用のプレイヤー
	player_ = new Player();
	player_->Initialize(camera_);

	//弾のモデル
	bulletObj.reset(ObjObject3d::Create());
	//弾のインターバル
	bulletIntervalTimer = resetTimer;

	//HPバーのモデル
	HP_UIObj.reset(ObjObject3d::Create());
	HP_UIObj->SetModel("HP_UI");

	HP_UITrans.Initialize();

	UITranslation = { 0.0f,10.0f,0.0f };

	HPScale = { 3.0f,3.0f,3.0f };

	//雑魚敵の初期HP
	enemyHP = 5;

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributeEnemy);
}

void EnemyNormal::Update()
{
	//デスフラグの立った弾を削除
	bullets.remove_if([ ] (EnemyBullet* bullet)
	{
		if ( bullet->IsDead() )
		{
			delete bullet;
			return true;
		}
		return false;
	});

	//各敵の更新
	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Update(camera_);
	}

	enemyTrans.SetTranslation(translation);
	enemyTrans.Update(camera_);
	enemyObj_->Update();

	HP_UITrans.SetTranslation({ translation.x + UITranslation.x,translation.y + UITranslation.y,translation.z + UITranslation.z });

	HP_UITrans.SetScale(HPScale);

	HP_UITrans.Update(camera_,true);

	//当たり判定を敵の原点に設定
	sphere.center = enemyTrans.GetTranslation();
}

void EnemyNormal::Draw()
{
	enemyObj_->Draw(&enemyTrans);

	HP_UIObj->Draw(&HP_UITrans);

	//弾の描画
	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Draw();
	}
}

void EnemyNormal::OnCollision()
{
	//弾発射
	Fire();

	//フラグが立った時回転させる
	if ( turnFlag )
	{
		Turn();
	}
}

void EnemyNormal::HitBullet()
{
	//HPを減らす
	enemyHP--;

	//ゼロになったら死亡フラグを立てる
	if ( enemyHP == zero )
	{
		isDead = true;
	}
}

bool EnemyNormal::IsDead() const
{
	return isDead;
}

const std::list<EnemyBullet*>& EnemyNormal::GetBullets() const
{
	return bullets;
}

void EnemyNormal::SetPlayer(Player* player)
{
	player_ = player;
}

void EnemyNormal::Fire()
{
	assert(player_);

	//タイマー作動
	bulletIntervalTimer--;

	//自キャラのワールド座標を取得する
	playerWorldPos = player_->GetCenterPosition();
	//敵キャラのワールド座標を取得する
	enemyWorldPos = GetCenterPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	enemyToPlayerVec = playerWorldPos - enemyWorldPos;
	//ベクトルの正規化
	enemyDir = MyMathUtility::MakeNormalize(enemyToPlayerVec);
	//ベクトルの長さを、早さに合わせる
	velocity = enemyDir * bulletSpeed;

	//タイマーがゼロになった時生成する
	if ( bulletIntervalTimer == zero )
	{
		//弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->SetPlayer(player_);
		newBullet->Initialize(bulletObj.get(),enemyTrans.GetTranslation(),velocity);

		//弾を登録する
		bullets.push_back(newBullet);
		bulletIntervalTimer = resetTimer;
	}
}

void EnemyNormal::Turn()
{
	MyMath::Vector3 rot;

	//角度を算出して自機方向に振り向かせる(Y軸)
	enemyAngleY = std::atan2(enemyDir.x,enemyDir.z);
	rot.y = MyMathUtility::LerpShortAngle(enemyTrans.GetRotation().y,enemyAngleY,0.3f);
	//Z軸の横軸
	MyMath::Vector3 vecLength = enemyDir;
	//Y成分を0にしたベクトル
	vecLength.y = 0;
	//角度を算出して自機方向に振り向かせる(X軸)
	enemyAngleX = std::atan2(-enemyDir.y,vecLength.length());
	rot.x = MyMathUtility::LerpShortAngle(enemyTrans.GetRotation().x,enemyAngleX,0.3f);
	//計算した角度をセット
	enemyTrans.SetRotation(rot);
}

MyMath::Vector3 EnemyNormal::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(enemyTrans);
	return worldPos;
}

