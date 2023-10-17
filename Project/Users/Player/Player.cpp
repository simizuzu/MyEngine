#include "Player.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file Player.cpp
 * @brief Playerの処理が書かれてあります
 */

void Player::Initialize(Camera* camera)
{
	assert(camera);

	input = Input::GetInstance();
	camera_.reset(camera);

	curveData = LevelLoader::LoadFile("curveTest");
	texReticle = TextureManager::Load("Resources/Texture/reticle2.png");
	sprite2DReticle = std::make_unique<Sprite>();
	sprite2DReticle->Initialize();

	playerModel.reset(ObjModel::LoadFromObj("Tyoinori"));
	bulletModel.reset(ObjModel::LoadFromObj("box"));
	reticleModel.reset(ObjModel::LoadFromObj("box"));
	playerObj.reset(ObjObject3d::Create());
	bulletObj.reset(ObjObject3d::Create());
	reticleObj.reset(ObjObject3d::Create());

	playerObj->SetModel(playerModel.get());
	reticleObj->SetModel(reticleModel.get());

	//プレイヤーのトランスフォーム初期化
	playerTrans.Initialize();
	//3Dレティクルのトランスフォーム初期化
	worldTransform3DReticle.Initialize();
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
		});

	//マウスカーソルの位置取得
	mousePos.x = input->GetMousePos().x;
	mousePos.y = input->GetMousePos().y;

#ifdef _DEBUG
	ImGui::Begin("MousePos");
	ImGui::Text("Position(%f,%f)", mousePos.x, mousePos.y);
	ImGui::End();

	ImGui::Begin("PlayerPos");
	ImGui::Text("Position(%f,%f,%f)",playerTrans.GetTranslation().x,playerTrans.GetTranslation().y,playerTrans.GetTranslation().z);
	ImGui::Text("worldTransform3DReticle(%f,%f,%f)",worldTransform3DReticle.GetTranslation().x,worldTransform3DReticle.GetTranslation().y,worldTransform3DReticle.GetTranslation().z);
	ImGui::End();
#endif

	//移動処理
	Move();
	playerTrans.Update(camera_.get());
	//攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets) {
		bullet->Update(camera_.get());
	}
		playerTrans.Update(camera_.get());
}

void Player::Draw()
{
	//playerObj->Draw(&playerTrans);

	reticleObj->Draw(&worldTransform3DReticle);

	//弾描画
	for (PlayerBullet* bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Move()
{
	//translation = mouseVec;
	
	PlayerTarget();

	//Reticle3D();
	//Reticle2D();
	ReticleMouse();

}

void Player::PlayerTarget()
{

}

void Player::UIDraw()
{
	sprite2DReticle->Draw(texReticle, sprite2DReticle->GetPosition(), {0.2f,0.2f}, 0.0f, {0.5f,0.5f});
}

void Player::Reticle3D()
{
	
}

void Player::Reticle2D()
{

}

void Player::ReticleMouse()
{
	
}

void Player::Attack()
{
	//弾の速度
	const float bulletSpeed = 1.0f;
	MyMath::Vector3 velosity(0, 0, bulletSpeed);

	if (input->MouseButtonTrigger(LEFT)) {

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel.get(), bulletObj.get(),mousePos,velosity);

		//弾を登録する
		bullets.push_back(newBullet);
	}
}

Player::~Player()
{
	for (PlayerBullet* bullet : bullets) {
		delete bullet;
	}
}
