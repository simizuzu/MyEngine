#include "Player.h"

void Player::Initialize(Camera* camera)
{
	assert(camera);

	input = Input::GetInstance();
	camera_.reset(camera);

	//curveData2 = LevelLoader::LoadFile("curveData");

	playerModel.reset(ObjModel::LoadFromObj("Tyoinori"));
	bulletModel.reset(ObjModel::LoadFromObj("Tyoinori"));
	playerObj.reset(ObjObject3d::Create());
	bulletObj.reset(ObjObject3d::Create());
	playerObj->SetModel(playerModel.get());

	playerTrans.Initialize();
}

void Player::Update()
{
	//playerTrans.SetTranslation(MyMathUtility::BezierCurve(curveData2->curves,t));

	//デスフラグの立った弾を削除
	bullets.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	playerTrans.Update(camera_.get());
	//playerTrans.SetScale({ 10.0f,10.0f,10.0f });
	playerTrans.SetTranslation({ 0 ,0,50.0f });

	if (input->PushKey(DIK_RIGHT)) {
		rot.y += 0.02f;
	}
	else if (input->PushKey(DIK_LEFT)) {
		rot.y -= 0.02f;
	}

	if (input->PushKey(DIK_A)) {
		translation.x -= 0.5f;
	}
	else if (input->PushKey(DIK_D)) {
		translation.x += 0.5f;
	}
	if (input->PushKey(DIK_W)) {
		translation.y += 0.5f;
	}
	else if (input->PushKey(DIK_S)) {
		translation.y -= 0.5f;
	}

	playerTrans.SetTranslation(translation);
	playerTrans.SetRotation(rot);

	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets) {
		bullet->Update(camera_.get());
	}
}

void Player::Draw()
{
	playerObj->Draw(&playerTrans);

	//弾描画
	for (PlayerBullet* bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Attack()
{	
	if (input->MouseButtonTrigger(LEFT)) {
		
		//弾の速度
		const float bulletSpeed = 1.0f;
		MyMath::Vector3 velosity(0,0,bulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velosity = MyMath::Vec3Mat4Mul(velosity, playerTrans.matWorld);
		
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel.get(), bulletObj.get(), playerTrans.GetTranslation(), velosity);

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
