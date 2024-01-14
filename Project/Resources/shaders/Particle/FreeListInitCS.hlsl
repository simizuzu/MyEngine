AppendStructuredBuffer<uint> freeList : register(u0);

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
	//�o�ߎ���
	float deltaTime;
	//�p�[�e�B�N���̍ő吔
	uint MAX_PARTICLE;
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= MAX_PARTICLE)
	{
		return;
	}

	freeList.Append(DTid.x);
}