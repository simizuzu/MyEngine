cbuffer cbuff0 : register(b0)
{
	matrix matWorld;
	matrix billMatWorld;
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

struct VSOutput
{
	float4 pos : POSITION;
	float size : SIZE;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};