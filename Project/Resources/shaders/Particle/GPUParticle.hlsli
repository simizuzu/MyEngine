cbuffer cbuff0 : register(b0)
{
	matrix matWorld;
	matrix matBillboard;
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
	float3 pos : POSITION;
	float size : SIZE;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

// �l�p�`�̒��_��
static const uint vnum = 4;

// �Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0), //����
	float4(-0.5f, +0.5f, 0, 0), //����
	float4(+0.5f, -0.5f, 0, 0), //�E��
	float4(+0.5f, +0.5f, 0, 0) //�E��
};

static const float2 uv_array[vnum] =
{
	float2(0, 1), //����
	float2(0, 0), //����
	float2(1, 1), //�E��
	float2(1, 0) //�E��
};