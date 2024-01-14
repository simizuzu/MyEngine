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