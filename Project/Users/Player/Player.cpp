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

	ImGui::Begin("CameraAngle");
	ImGui::Text("CameraAngle(%f,%f)",cameraHAngle,cameraVAngle );
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
	reticleObj->Draw(&worldTransform3DReticle);

	//弾描画
	for (PlayerBullet* bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Move()
{
	//translation = mouseVec;
	Reticle3D();
	Reticle2D();
	ReticleMouse();
}

void Player::UIDraw()
{
	sprite2DReticle->Draw(texReticle, sprite2DReticle->GetPosition(), {0.2f,0.2f}, 0.0f, {0.5f,0.5f});
}

void Player::Reticle3D()
{
	//自機から3Dレティクルへの距離
	const float distancePlayerTo3DReticle = 10.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	MyMath::Vector3 offset = { 0,0,1.0f };
	//カメラの角度を取得する
	cameraHAngle = camera_->GetHAngle(camera_->GetEye(),camera_->GetTarget()); //水平方向
	cameraVAngle = camera_->GetVAngle(camera_->GetEye(),camera_->GetTarget()); //垂直方向
	
	//自機のワールド行列の回転を反映
	offset = MyMath::Vec3Mat4Mul(offset,playerTrans.matWorld);
	//ベクトルの長さを整える
	offset = MyMathUtility::MakeNormalize(offset) * distancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle.translation_ = MyMath::GetWorldPosition(playerTrans) + offset;
	worldTransform3DReticle.Update(camera_.get());
}

void Player::Reticle2D()
{
	const MyMath::Matrix4 matView = camera_->GetMatView();
	const MyMath::Matrix4 matProjection = camera_->GetMatProjection();

	//3Dレティクルのワールド行列から、ワールド座標を取得
	MyMath::Vector3 positionReticle = MyMath::GetWorldPosition(worldTransform3DReticle);
	//ビューポート行列
	MyMath::Matrix4 matViewport = MyMathUtility::MakeViewport(matViewport);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	MyMath::Matrix4 matViewProViewport = matView * matProjection * matViewport;
	//ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	positionReticle = MyMathUtility::MakeWDivision(positionReticle,matViewProViewport);
	//スプライトのレティクルに座標設定
	sprite2DReticle->SetPosiotion(MyMath::Vector2(positionReticle.x,positionReticle.y));
}

void Player::ReticleMouse()
{
	const MyMath::Matrix4 matView = camera_->GetMatView();
	const MyMath::Matrix4 matProjection = camera_->GetMatProjection();

	//マウス座標の取得
	sprite2DReticle->SetPosiotion(MyMath::Vector2(mousePos.x,mousePos.y));

	//3Dレティクルのワールド行列から、ワールド座標を取得
	MyMath::Vector3 positionReticle = MyMath::GetWorldPosition(worldTransform3DReticle);
	//ビューポート行列
	MyMath::Matrix4 matViewport = MyMathUtility::MakeViewport(matViewport);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	MyMath::Matrix4 matViewProViewport = matView * matProjection * matViewport;
	//合成行列の逆行列を計算する
	MyMath::Matrix4 matInverseVPV = MyMathUtility::MakeInverse(matViewProViewport);

	//スクリーン座標
	MyMath::Vector3 posNear = MyMath::Vector3(mousePos.x,mousePos.y,0);
	MyMath::Vector3 posFar = MyMath::Vector3(mousePos.x,mousePos.y,1);
	//スクリーン座標景からワールド座標系へ
	posNear = MyMathUtility::MakeWDivision(posNear,matInverseVPV);
	posFar = MyMathUtility::MakeWDivision(posFar,matInverseVPV);

	//マウスレイの方向
	MyMath::Vector3 mouseDirection = posFar - posNear;
	mouseDirection = MyMathUtility::MakeNormalize(mouseDirection);
	//カメラから標準オブジェクトの距離
	const float distanceObject = 50.0f;
	worldTransform3DReticle.translation_ = posNear + mouseDirection * distanceObject;
	worldTransform3DReticle.Update(camera_.get());
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
