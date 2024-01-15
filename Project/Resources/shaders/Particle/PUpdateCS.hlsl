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
AppendStructuredBuffer<uint> freeList : register(u1);
RWStructuredBuffer<uint> DrawList : register(u2);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= MAX_PARTICLE)
	{
		return;
	}

	ParticleData particle = ParticlePool.Load(DTid.x);
	
	if (particle.isAlive == 0.0f)
	{
		return;
	}
	
	float t = particle.AGE / particle.lifeTime;
	
	particle.AGE += deltaTime;
	
	particle.isAlive = (float) (particle.AGE < particle.lifeTime);
	
	particle.position += particle.velocity * deltaTime;
	
	particle.velocity += particle.accel;
	
	particle.color = lerp(startColor, endColor, t);

	particle.size = lerp(scale.x, scale.r, t);
	
	ParticlePool[DTid.x] = particle;
	
	if (particle.isAlive == 0.0f)
	{
		freeList.Append(DTid.x);
	}
	else
	{
		uint drawIndex = DrawList.IncrementCounter();

		uint drawData;
		drawData = DTid.x;
		
		DrawList[drawIndex] = drawData;
	}
}