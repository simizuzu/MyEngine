#include "EnemyNormal.h"
#include "Numbers.h"

#include "Player.h"

#include "ModelManager.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

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
	enemyTrans.SetScale({ 0.009f,0.009f ,0.009f });

	//自機狙い用のプレイヤー
	player_ = new Player();
	player_->Initialize(camera_);

	//弾のモデル
	bulletObj.reset(ObjObject3d::Create());
	//弾のインターバル
	bulletIntervalTimer = resetTimer;

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

	//当たり判定を敵の原点に設定
	sphere.center = enemyTrans.GetTranslation();
}

void EnemyNormal::Draw()
{
	enemyObj_->Draw(&enemyTrans);

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
}

void EnemyNormal::HitBullet()
{
	//HPを減らす
	enemyHP--;

	//ゼロになった時フラグを立てる
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

	//弾の速度
	const float bulletSpeed = 10.0f;
	MyMath::Vector3 velocity(0,0,bulletSpeed);

	//自キャラのワールド座標を取得する
	MyMath::Vector3 playerWorldPos = player_->GetCenterPosition();
	//敵キャラのワールド座標を取得する
	MyMath::Vector3 enemyWorldPos = GetCenterPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	MyMath::Vector3 enemyToPlayerVec = playerWorldPos - enemyWorldPos;
	//ベクトルの正規化
	MyMath::Vector3 enemyDir = MyMathUtility::MakeNormalize(enemyToPlayerVec);
	//ベクトルの長さを、早さに合わせる
	velocity = enemyDir * bulletSpeed;

	//タイマーがゼロになった時生成する
	if ( bulletIntervalTimer == zero )
	{
		//弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(bulletObj.get(),enemyTrans.GetTranslation(),velocity);

		//弾を登録する
		bullets.push_back(newBullet);
		bulletIntervalTimer = resetTimer;
	}

//#ifdef _DEBUG
//	ImGui::Begin("WorldPos");
//	ImGui::Text("plyerPos(%f,%f,%f)",playerWorldPos.x,playerWorldPos.y,playerWorldPos.z);
//	ImGui::Text("enemyPos (%f,%f,%f)",enemyWorldPos.x,enemyWorldPos.y,enemyWorldPos.z);
//	ImGui::End();
//#endif
}

MyMath::Vector3 EnemyNormal::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(enemyTrans);
	return worldPos;
}

