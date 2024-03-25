#include "DamegeEffect.h"
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cmath>
#include <random>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class DmageEffect.cpp
 * @brief BaseParticleを継承したヒット時のパーティクル演出
 */

void DamegeEffect::Initialize(ObjModel* model,Camera* camera)
{
	assert(camera);
	camera_=camera;

	colorStart = 0.1f;
	colorEnd = 0.8f;
	alphaStart = 1.0f;
	alphaEnd = 0.0f;

	obj.reset(ObjObject3d::Create());
	obj->SetModel(model);

	transform.Initialize();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937 engine(seed_gen());

	//乱数
	std::uniform_real_distribution<float> posDist(-0.75f,0.75f);
	std::uniform_real_distribution<float> valueDistXZ(-0.2f,0.2f);
	std::uniform_real_distribution<float> valueDistY(-0.2f,0.2f);
	std::uniform_real_distribution<float> rotValue(-180.0f,180.0f);

	//乱数エンジンを渡し、指定範囲かつランダムな数値を得る
	MyMath::Vector3 pos = { 0,0,0 };
	pos = { posDist(engine),0.0f,posDist(engine) };
	oneGrain.pos += pos;

	//xyzをランダムに分布
	MyMath::Vector3 value = { 0,0,0 };
	value = { valueDistXZ(engine),valueDistY(engine),valueDistXZ(engine) };

	oneGrain.velocity = value;

	MyMath::Vector3 rot = { 0,0,0 };
	rot = { rotValue(engine),rotValue(engine),rotValue(engine) };

	transform.SetTranslation(oneGrain.pos);
	transform.SetRotation(oneGrain.rot);
}

void DamegeEffect::Update()
{
	oneGrain.startFrame++;

	//速度による移動
	oneGrain.pos += oneGrain.velocity * speed;
	//進行度を0～1の範囲に換算
	float f = ( float ) oneGrain.startFrame / oneGrain.endFrame;
	//スケールの線形補間
	oneGrain.scale = ( oneGrain.endScale - oneGrain.startScale ) * f;

	transform.SetTranslation(oneGrain.pos);
	transform.SetScale({ oneGrain.scale, oneGrain.scale, oneGrain.scale });
	transform.Update(camera_);
}

void DamegeEffect::Draw()
{
	obj->Draw(&transform);
}
