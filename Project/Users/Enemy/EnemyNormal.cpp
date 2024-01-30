#include "EnemyNormal.h"
#include "Numbers.h"

#include "Player.h"

#include "ModelManager.h"

EnemyNormal::~EnemyNormal()
{
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

	enemyObj_.reset(FbxObject3d::Create());
	enemyObj_->SetModel(modelName);
	enemyObj_->PlayAnimation();

	enemyTrans.Initialize();
	enemyTrans.SetScale({ 0.009f,0.009f ,0.009f });
	//enemyObj_->SetScale({ 0.009f,0.009f ,0.009f });

	player_ = new Player();
	player_->Initialize(camera_);

	bulletObj.reset(ObjObject3d::Create());

	bulletIntervalTimer = resetTimer;

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


	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Update(camera_);
	}

	//弾発射
	Fire();

	enemyTrans.SetTranslation(translation);
	enemyTrans.Update(camera_);
	enemyObj_->Update();
}

void EnemyNormal::Draw()
{
	enemyObj_->Draw(&enemyTrans);
	//colliderObj_->Draw(&colliderTrans);	

	//弾の描画
	for ( EnemyBullet* bullet : bullets )
	{
		bullet->Draw();
	}
}

void EnemyNormal::OnCollision()
{
	isDead = true;
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
		newBullet->Initialize(bulletObj.get(),enemyTrans.GetTranslation(),velocity);

		//弾を登録する
		bullets.push_back(newBullet);
		bulletIntervalTimer = resetTimer;
	}
}

MyMath::Vector3 EnemyNormal::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(enemyTrans);
	return worldPos;
}

