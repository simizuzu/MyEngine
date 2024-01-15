#include "HLSLMath.hlsli"

cbuffer EmitData : register(b0)
{
	//���W
	float3 pos;
	//����
	float lifeTime;
	//�����x
	float3 accel;
	//�X�P�[��
	float scale;
	//�J�n���̐F
	float4 startColor;
	//�I�����̐F
	float4 endColor;
	//���x
	float3 velocity;
	//�o�ߎ���
	float deltaTime;
	//�p�[�e�B�N���̍ő吔
	uint MAX_PARTICLE;
}

struct ParticleData
{
	//�F
	float4 color;
	//���W
	float3 position;
	//�o�ߎ���
	float AGE;
	//���x
	float3 velocity;
	//�������Ă��邩
	float isAlive;
	//�����x
	float3 accel;
	//�T�C�Y
	float size;
	//���C�t�^�C��
	float lifeTime;

	//�p�f�B���O
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