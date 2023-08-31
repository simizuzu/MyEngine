#include "Player.h"

void Player::Initialize(Camera* camera)
{
	assert(camera);

	input = Input::GetInstance();
	camera_.reset(camera);

	//curveData2 = LevelLoader::LoadFile("curveData");

	playerModel.reset(ObjModel::LoadFromObj("Tyoinori"));
	playerObj.reset(ObjObject3d::Create());
	playerObj->SetModel(playerModel.get());

	playerTrans.Initialize();
}

void Player::Update()
{
	//playerTrans.SetTranslation(MyMathUtility::BezierCurve(curveData2->curves,t));

	playerTrans.Update(camera_.get());
	playerTrans.SetScale({ 10.0f,10.0f,10.0f });
	playerTrans.SetTranslation({ 0 ,0,50.0f });

	if (input->PushKey(DIK_RIGHT)) {
		rot.y += 0.02f;
	}
	else if (input->PushKey(DIK_LEFT)) {
		rot.y -= 0.02f;
	}

	playerTrans.SetRotation(rot);
}

void Player::Draw()
{
	playerObj->Draw(&playerTrans);
}

void Player::Attack()
{
	if (input->MouseButtonTrigger(LEFT)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize()
	}
}
