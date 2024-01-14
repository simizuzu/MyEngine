#include "HLSLMath.hlsli"

cbuffer EmitData : register(b0)
{
	//座標
	float3 pos;
	//寿命
	float lifeTime;
	//加速度
	float3 accel;
	//スケール
	float scale;
	//開始時の色
	float4 startColor;
	//終了時の色
	float4 endColor;
	//速度
	float3 velocity;
	//経過時間
	float deltaTime;
	//パーティクルの最大数
	uint MAX_PARTICLE;
}

struct ParticleData
{
	//色
	float4 color;
	//座標
	float3 position;
	//経過時間
	float AGE;
	//速度
	float3 velocity;
	//生存しているか
	float isAlive;
	//加速度
	float3 accel;
	//サイズ
	float size;
	//ライフタイム
	float lifeTime;

	//パディング
	float3 pad;
};

RWStructuredBuffer<ParticleData> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= MAX_PARTICLE)
	{
		return;
	}

	uint emitIndex = freeList.Consume();
	
	ParticleData emitParticle = ParticlePool.Load(emitIndex);
	
	float3 rnd = RandVec3((int) emitIndex, 1, 0);
	
	emitParticle.position = (rnd * pos) - (pos / 2.0f);
	emitParticle.velocity = (rnd * velocity) - (velocity / 2.0f);
	emitParticle.color = startColor;
	emitParticle.accel = rnd * accel;
	emitParticle.AGE = 0.0f;
	emitParticle.size = 0.5f;
	emitParticle.isAlive = 1.0f;
	emitParticle.lifeTime = lifeTime;
	
	ParticlePool[emitIndex] = emitParticle;
}