#include "EnemyNormal.h"
#include "Numbers.h"

#include "Player.h"

EnemyNormal::~EnemyNormal()
{
	for ( EnemyBullet* bullet : bullets )
	{
		delete bullet;
	}
}

void EnemyNormal::Initialize(FbxModel* model,Camera* camera)
{
	//渡されたかチェック
	assert(camera);
	assert(model);
	camera_ = camera;
	enemyModel_ = model;

	enemyObj_.reset(FbxObject3d::Create());
	enemyObj_->SetModel(enemyModel_);
	enemyObj_->PlayAnimation();
	enemyObj_->SetScale({ 0.009f,0.009f ,0.009f });

	player_ = new Player();
	player_->Initialize(camera_);

	bulletModel.reset(ObjModel::LoadFromObj("box"));
	bulletObj.reset(ObjObject3d::Create());

	bulletIntervalTimer = resetTimer;
}

void EnemyNormal::Update()
{
	//デスフラグの立った弾を削除
	bullets.remove_if([ ](EnemyBullet* bullet) {
		if ( bullet->IsDead() )
		{
			delete bullet;
			return true;
		}
		return false;
	});

	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Update(camera_);
	}

	//弾発射
	Fire();

	enemyObj_->SetTranslation(translation);
	enemyObj_->Update(camera_);
}

void EnemyNormal::Draw()
{
	enemyObj_->Draw();

	//弾の描画
	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Draw();
	}
}

void EnemyNormal::OnCollision()
{
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

	bulletIntervalTimer--;

	//弾の速度
	const float bulletSpeed = 10.0f;
	MyMath::Vector3 velocity(0,0,bulletSpeed);

	//自キャラのワールド座標を取得する
	MyMath::Vector3 playerWorldPos = player_->GetPlayerWorldPosition();
	//敵キャラのワールド座標を取得する
	MyMath::Vector3 enemyWorldPos = enemyObj_->GetWorldPosition(enemyWorldPos);
	//敵キャラ→自キャラの差分ベクトルを求める
	MyMath::Vector3 enemyToPlayerVec = playerWorldPos - enemyWorldPos;
	//ベクトルの正規化
	MyMath::Vector3 enemyDir = MyMathUtility::MakeNormalize(enemyToPlayerVec);
	//ベクトルの長さを、早さに合わせる
	velocity = enemyDir * bulletSpeed;

	if ( bulletIntervalTimer == zero )
	{
		//弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(bulletModel.get(),bulletObj.get(),enemyObj_->GetTranslation(),velocity);

		//弾を登録する
		bullets.push_back(newBullet);
		bulletIntervalTimer = resetTimer;
	}
}

//MyMath::Vector3 EnemyNormal::GetEnemyWorldPostition()
//{
//	MyMath::Vector3 worldPos;
//
//	MyMathUtility::MakeTranslation(worldPos);
//
//	return worldPos;
//}
