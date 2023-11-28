#include "ParticleManager.h"

#include "DamegeEffect.h"

/**
 * @file ParticleManager.h
 * @brief パーティクルマネージャクラス
 */

void ParticleManager::Initialize(ObjModel* model, Camera* camera)
{
	assert(model);
	assert(camera);
	model_.reset(model);
	camera_=camera;
}

void ParticleManager::Update()
{
	//フラグがたったら削除
	particles.remove_if([ ](std::unique_ptr<BaseParticle>& p) {
		return p->oneGrain.startFrame >= p->oneGrain.endFrame;
		});

	for ( std::unique_ptr<BaseParticle>& p : particles )
	{
		p->Update();
	}
}

void ParticleManager::Draw()
{
	for ( std::unique_ptr<BaseParticle>& p : particles )
	{
		p->Draw();
	}
}

void ParticleManager::Add(const std::string& name,uint16_t amout,uint8_t life,MyMath::Vector3 pos,float startScale,float endScale)
{
	for ( size_t i = 0; i < amout; i++ )
	{
		std::unique_ptr <BaseParticle> p;
		if ( name == "1" )
		{
			p = std::make_unique <DamegeEffect>();
		}

		p->oneGrain.pos = pos;
		p->oneGrain.endFrame = life;
		p->oneGrain.startScale = startScale;
		p->oneGrain.endScale = endScale;
		p->Initialize(model_.get(),camera_);
		particles.emplace_front(std::move(p));
	}
}
